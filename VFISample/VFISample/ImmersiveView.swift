//
//  ImmersiveView.swift
//  CleansheetCloudXRClient
//
//  Created by Andrew Wilson on 8/28/24.
//

import SwiftUI
@preconcurrency import RealityKit
import os.log

@preconcurrency import CloudXRKit

let immersiveTitle = "CloudXRImmersive"

@Observable @MainActor
class AppModel {
    private static let logger = Logger(
        subsystem: Bundle.main.bundleIdentifier ?? "AppModel",
        category: "AppModel"
    )

    public var cloudXRSession: CloudXRKit.Session?

    public var stateManager: OmniverseStateManager = OmniverseStateManager()
    public static var omniverseMessageDispatcher = OmniverseMessageDispatcher()

    /// The MessageChannel used for all client↔server application messages.
    public var messageChannel: MessageChannel?

    init(cxrSession: Session) {
        self.cloudXRSession = cxrSession
        Self.omniverseMessageDispatcher.session = cxrSession
    }

    /// After CloudXR connection, wait for the server to announce a message channel,
    /// then cache it and wire up the dispatcher to its receive stream.
    func setupMessageChannel() async {
        guard let session = cloudXRSession else { return }

        // Poll for available channels (server announces them shortly after connect)
        for _ in 0..<50 {
            if let channelInfo = session.availableMessageChannels.first,
               let channel = session.getMessageChannel(channelInfo) {
                self.messageChannel = channel
                self.stateManager.messageChannel = channel
                Self.omniverseMessageDispatcher.messageChannel = channel
                Self.logger.info("MessageChannel ready (status: \(channel.status.rawValue))")
                return
            }
            try? await Task.sleep(for: .milliseconds(100))
        }
        Self.logger.error("No MessageChannel became available after 5 seconds")
    }

    /// Clean up message channel references on disconnect.
    func teardownMessageChannel() {
        messageChannel = nil
        stateManager.messageChannel = nil
        Self.omniverseMessageDispatcher.messageChannel = nil
    }

    /// Send application data to the Omniverse server via the MessageChannel.
    func sendMessage(_ data: Data) {
        if let channel = messageChannel {
            _ = channel.sendServerMessage(data)
        } else {
            cloudXRSession?.sendServerMessage(data)
        }
    }
}

struct StreamingView: View {
    private static let logger = Logger(
        subsystem: Bundle.main.bundleIdentifier ?? "StreamingView",
        category: "StreamingView"
    )

    @Environment(\.openImmersiveSpace) var openImmersiveSpace
    @Environment(AppModel.self) var appModel

    @State private var cloudXRSessionEntity = Entity()

    // PRIMS
    @State private var dgxPrim = Entity()
    @State private var sledPrim = Entity()
    @State private var lineConveyPrim = Entity()
    @State public var lastEntityTapped : Entity?

    // Gesture tracking state
    @State private var lastDragTranslation: SIMD3<Float> = .zero

    /// Sensitivity: converts meters of 3D drag to meters sent to the server.
    private let dragSensitivity: Float = 2.0

    private var tapGesture: some Gesture {
        return SpatialTapGesture()
            .targetedToAnyEntity()
            .onEnded { tappedEntity in
                let rawEntity = tappedEntity.entity
                Self.logger.info("Tap detected on entity: '\(rawEntity.name)'")

                // Resolve to the entity that owns OmniversePrimComponent.
                // The tap may land on the bounding-box child rather than
                // the prim parent, so walk up one level if needed.
                let primEntity: Entity
                if rawEntity.components[OmniversePrimComponent.self] != nil {
                    primEntity = rawEntity
                } else if let parent = rawEntity.parent,
                          parent.components[OmniversePrimComponent.self] != nil {
                    primEntity = parent
                } else {
                    Self.logger.info("Tapped entity has no OmniversePrimComponent — ignoring")
                    return
                }

                // Deselect previous entity: remove highlight, hide text
                if let previousEntity = lastEntityTapped {
                    if !previousEntity.children.isEmpty {
                        let bbox = previousEntity.children[0]
                        bbox.components.remove(HoverEffectComponent.self)
                        if !bbox.children.isEmpty {
                            bbox.children[0].isEnabled = false
                        }
                    }
                }

                // Select new entity
                if !primEntity.children.isEmpty {
                    let bbox = primEntity.children[0]
                    bbox.components.set(HoverEffectComponent(.highlight(
                        HoverEffectComponent.HighlightHoverEffectStyle(
                            color: .green, strength: 15.0
                        )
                    )))
                    if !bbox.children.isEmpty {
                        bbox.children[0].isEnabled = true
                        bbox.children[0].setText("Updating")
                    }
                }

                lastEntityTapped = primEntity
                Self.logger.info("Sending PrimTap for: '\(primEntity.name)'")
                appModel.sendMessage(encodeJSON(PrimTapInputEvent(primEntity.name)))
            }
    }

    /// Drag gesture for camera orbit / prim translation.
    /// Targeted to entities so visionOS delivers events in immersive space.
    private var dragGesture: some Gesture {
        DragGesture()
            .targetedToAnyEntity()
            .onChanged { value in
                let translation3D = value.convert(value.translation3D, from: .local, to: .scene)
                let delta = translation3D - lastDragTranslation
                lastDragTranslation = translation3D

                let dx = Float(delta.x) * dragSensitivity
                let dy = Float(delta.y) * dragSensitivity

                Self.logger.info("Drag changed: dx=\(dx), dy=\(dy)")
                appModel.sendMessage(
                    encodeJSON(DragInputEvent(deltaX: dx, deltaY: dy, deltaZ: 0, phase: "changed"))
                )
            }
            .onEnded { _ in
                lastDragTranslation = .zero
                Self.logger.info("Drag ended")
                appModel.sendMessage(
                    encodeJSON(DragInputEvent(deltaX: 0, deltaY: 0, deltaZ: 0, phase: "ended"))
                )
            }
    }

    // NOTE: MagnifyGesture removed — on visionOS, hand-tracking jitter
    // causes it to fire continuously on the large gestureTarget sphere,
    // blocking tap and drag from ever recognizing. Camera zoom can be
    // driven via the Bay camera UI or a future two-hand gesture on
    // smaller, prim-specific collision targets.

    var body: some View {

        RealityView { content in
            cloudXRSessionEntity.name = "Session"
            guard let session = appModel.cloudXRSession else {
                return
            }

            cloudXRSessionEntity.components[CloudXRSessionComponent.self] = .init(session: session)
            cloudXRSessionEntity.transform = .init()
            content.add(cloudXRSessionEntity)

            // No gestureTarget entity — a surrounding sphere always wins the
            // hit test (user is inside it), and a backdrop plane requires
            // knowing the scene orientation.  Instead, tap and drag target
            // the prim entities directly via their collision shapes.
            //
            // Each prim needs both InputTargetComponent AND CollisionComponent
            // for RealityKit hit-testing. The collision shape here is a
            // placeholder; drawPrimBoundingBox will add a precise shape on
            // the child entity once the server sends bounding-box data.
            dgxPrim.components[OmniversePrimComponent.self] = OmniversePrimComponent(primPath:"/World/DGX_Tray")
            dgxPrim.components.set(InputTargetComponent(allowedInputTypes: .all))
            dgxPrim.components.set(CollisionComponent(shapes: [.generateSphere(radius: 1.5)], isStatic: true))
            content.add(dgxPrim)
            sledPrim.components[OmniversePrimComponent.self] = OmniversePrimComponent(primPath:"/World/ProdLine/assembly_ProdLine_V02/V02/V02Machines/Line_SLED/SLED/ASSET/id_LS10RobotArm")
            sledPrim.components.set(InputTargetComponent(allowedInputTypes: .all))
            sledPrim.components.set(CollisionComponent(shapes: [.generateSphere(radius: 1.5)], isStatic: true))
            content.add(sledPrim)
            lineConveyPrim.components[OmniversePrimComponent.self] = OmniversePrimComponent(primPath:"/World/ProdLine/assembly_ProdLine_V02/V02/V02Machines/Line_Convey/ProductElevatorTowerTypeA_106x150x213_01")
            lineConveyPrim.components.set(InputTargetComponent(allowedInputTypes: .all))
            lineConveyPrim.components.set(CollisionComponent(shapes: [.generateSphere(radius: 1.5)], isStatic: true))
            content.add(lineConveyPrim)

            Self.logger.info("Gesture setup complete — 3 prims with InputTarget+Collision(r=1.5), no gestureTarget")
        }
        .gesture(tapGesture.exclusively(before: dragGesture))
    }
}
