//
//  ImmersiveView.swift
//  CleansheetCloudXRClient
//
//  Created by Andrew Wilson on 8/28/24.
//

import SwiftUI
@preconcurrency import RealityKit

@preconcurrency import CloudXRKit

let immersiveTitle = "CloudXRImmersive"

@Observable @MainActor
class AppModel {
    public var cloudXRSession: CloudXRKit.Session?

    public var stateManager: OmniverseStateManager = OmniverseStateManager()
    public static var omniverseMessageDispatcher = OmniverseMessageDispatcher()

    init(cxrSession: Session) {
        self.cloudXRSession = cxrSession
        Self.omniverseMessageDispatcher.session = cxrSession
    }
}

struct StreamingView: View {
    @Environment(\.openImmersiveSpace) var openImmersiveSpace
    @Environment(AppModel.self) var appModel

    @State private var cloudXRSessionEntity = Entity()

    // PRIMS
    @State private var dgxPrim = Entity()
    @State private var sledPrim = Entity()
    @State private var lineConveyPrim = Entity()
    @State public var lastEntityTapped : Entity?

    // Gesture tracking state
    @State private var lastDragTranslation: CGSize = .zero
    @State private var lastMagnification: CGFloat = 1.0

    /// Sensitivity: converts points of drag to meters sent to the server.
    private let metersPerPoint: Float = 0.0005
    /// Sensitivity: converts magnification delta to meters of camera dolly.
    private let zoomMetersPerUnit: Float = 0.5

    private var tapGesture: some Gesture {
        return SpatialTapGesture()
            .targetedToAnyEntity()
            .onEnded { tappedEntity in
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

                // Select new entity: only act on entities with OmniversePrimComponent
                let entity = tappedEntity.entity
                guard entity.components[OmniversePrimComponent.self] != nil ||
                      entity.parent?.components[OmniversePrimComponent.self] != nil else {
                    return
                }

                if !entity.children.isEmpty {
                    let bbox = entity.children[0]
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

                lastEntityTapped = entity
                appModel.cloudXRSession?.sendServerMessage(encodeJSON(PrimTapInputEvent(entity.name)))
            }
    }

    /// Drag gesture for camera orbit / prim translation.
    /// The server decides the mode based on whether a prim is currently selected.
    private var dragGesture: some Gesture {
        DragGesture(minimumDistance: 10)
            .onChanged { value in
                let deltaW = Float(value.translation.width - lastDragTranslation.width)
                let deltaH = Float(value.translation.height - lastDragTranslation.height)
                lastDragTranslation = value.translation

                let dx = deltaW * metersPerPoint
                let dy = -deltaH * metersPerPoint  // Flip Y: screen down is negative in world

                appModel.cloudXRSession?.sendServerMessage(
                    encodeJSON(DragInputEvent(deltaX: dx, deltaY: dy, deltaZ: 0, phase: "changed"))
                )
            }
            .onEnded { _ in
                lastDragTranslation = .zero
                appModel.cloudXRSession?.sendServerMessage(
                    encodeJSON(DragInputEvent(deltaX: 0, deltaY: 0, deltaZ: 0, phase: "ended"))
                )
            }
    }

    /// Pinch-to-zoom gesture — always dollies the camera.
    private var zoomGesture: some Gesture {
        MagnifyGesture()
            .onChanged { value in
                let delta = Float(value.magnification - lastMagnification)
                lastMagnification = value.magnification

                let zoomDelta = delta * zoomMetersPerUnit

                appModel.cloudXRSession?.sendServerMessage(
                    encodeJSON(ZoomInputEvent(delta: zoomDelta, phase: "changed"))
                )
            }
            .onEnded { _ in
                lastMagnification = 1.0
                appModel.cloudXRSession?.sendServerMessage(
                    encodeJSON(ZoomInputEvent(delta: 0, phase: "ended"))
                )
            }
    }

    var body: some View {

        RealityView { content in
            cloudXRSessionEntity.name = "Session"
            guard let session = appModel.cloudXRSession else {
                return
            }

            cloudXRSessionEntity.components[CloudXRSessionComponent.self] = .init(session: session)
            cloudXRSessionEntity.transform = .init()
            content.add(cloudXRSessionEntity)

            dgxPrim.components[OmniversePrimComponent.self] = OmniversePrimComponent(primPath:"/World/DGX_Tray")
            dgxPrim.components.set(InputTargetComponent(allowedInputTypes: .all))
            content.add(dgxPrim)
            sledPrim.components[OmniversePrimComponent.self] = OmniversePrimComponent(primPath:"/World/ProdLine/assembly_ProdLine_V02/V02/V02Machines/Line_SLED/SLED/ASSET/id_LS10RobotArm")
            sledPrim.components.set(InputTargetComponent(allowedInputTypes: .all))
            content.add(sledPrim)
            lineConveyPrim.components[OmniversePrimComponent.self] = OmniversePrimComponent(primPath:"/World/ProdLine/assembly_ProdLine_V02/V02/V02Machines/Line_Convey/ProductElevatorTowerTypeA_106x150x213_01")
            lineConveyPrim.components.set(InputTargetComponent(allowedInputTypes: .all))
            content.add(lineConveyPrim)
        }
        .gesture(tapGesture)
        .gesture(dragGesture)
        .gesture(zoomGesture)
    }
}
