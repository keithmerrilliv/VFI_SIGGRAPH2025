//
//  ContentView.swift
//  Cleansheet CloudXR Client
//
//  Created by Andrew Wilson on 8/28/24.
//

import SwiftUI
import os.log
import GroupActivities

@preconcurrency import CloudXRKit

struct ContentView: View {
    private static let logger = Logger()

    enum AppState: String {
        case launch = "Initialized"
        case connected = "Connected"
        case disconnected = "Disconnected"
        case paused = "Paused"
    }

    enum UIPage {
        case config
        case controls
        case sharePlay
    }

    // Configurable session settings.
    @AppStorage("ipAddress") private var ipAddress: String = ""
    @AppStorage("resolutionPreset") private var resolutionPreset: ResolutionPreset = .standardPreset

    @State private var appState: AppState = .launch
    @State var uiPage = UIPage.config

    @Environment(\.openImmersiveSpace) var openImmersiveSpace
    @Environment(AppModel.self) var appModel

    var buttonLabel: String {
        switch appState {
        case .launch, .disconnected:
            return "Connect"
        case .connected:
            return "Disconnect"
        case .paused:
            return "Resume"
        }
    }

    @State private var buttonDisabled: Bool = false

    @State private var selection = BayInputCases.Bay1
    @State private var playButtonPressed: Bool = false
    @State private var bayNumber: Int = 0

    /// Suppresses SharePlay broadcast when we're applying a remote participant's state.
    @State private var isApplyingRemoteState: Bool = false

    var body: some View {
        VStack {

            switch uiPage {
            case .config:

                Form {
                    HStack {
                        Text("IP Address")
                        Spacer()
                        TextField("0.0.0.0", text: $ipAddress)
                            .autocorrectionDisabled(true)
                            .textInputAutocapitalization(.never)
                            .keyboardType(.decimalPad)
                            .searchDictationBehavior(.inline(activation: .onLook))
                            .onSubmit {
                                ipAddress = ipAddress.trimmingCharacters(
                                    in: .whitespacesAndNewlines
                                )
                            }
                    }
                    Picker("Resolution Preset", selection: $resolutionPreset) {
                        ForEach(ResolutionPreset.allCases, id: \.self) { preset in
                            Text(preset.rawValue)
                        }
                    }.disabled(appState != .launch)
                }
                Spacer()
                Text(appState.rawValue)
                Spacer()
                Button(buttonLabel) {
                    Task { @MainActor in
                        buttonDisabled = true
                        defer { buttonDisabled = false }

                        guard let session = appModel.cloudXRSession else {
                            return
                        }

                        if appState == .launch || appState == .disconnected {
                            var config = CloudXRKit.Config()
                            config.connectionType = .local(ip: ipAddress)
                            session.configure(config: config)
                            appModel.stateManager.session = session
                            do {
                                try await session.connect()
                                appState = .connected
                                uiPage = .controls
                                await openImmersiveSpace(id: immersiveTitle)

                                // Broadcast connection to SharePlay participants
                                broadcastConnection(action: .connect)
                            } catch {
                                Self.logger.error("Failed to connect: \(error.localizedDescription)")
                                appState = .disconnected
                            }
                            return
                        }

                        if appState == .connected {
                            session.disconnect()
                            appState = .disconnected
                            broadcastConnection(action: .disconnect)
                            return
                        }
                    }
                }.disabled(buttonDisabled).padding(25)
            case .controls:
                Spacer()
                Text("Animation Controls")

                HStack {
                    Button(playButtonPressed ? "Pause" : "Play") {
                        let action: AnimationCases = playButtonPressed ? .Pause : .Play
                        appModel.stateManager["animationAction"] = action
                        self.playButtonPressed.toggle()
                        broadcastControl(type: .animation, value: action.rawValue)
                    }
                    Button("Stop") {
                        appModel.stateManager["animationAction"] = AnimationCases.Stop
                        self.playButtonPressed = false
                        broadcastControl(type: .animation, value: AnimationCases.Stop.rawValue)
                    }
                    Button("Rewind") {
                        appModel.stateManager["animationAction"] = AnimationCases.Rewind
                        self.playButtonPressed = true
                        broadcastControl(type: .animation, value: AnimationCases.Rewind.rawValue)
                    }
                }.disabled(appState != .connected)

                Spacer()

                Text("Bay Camera Selection")

                HStack {
                    Button("Previous") {
                        bayNumber -= 1
                        if(bayNumber <= -1) {
                            bayNumber = 6
                        }
                        selection = BayInputCases.allCases[bayNumber]
                        appModel.stateManager["bay"] = BayInputCases.allCases[bayNumber]
                        broadcastControl(type: .bay, value: selection.rawValue)
                    }
                    Picker("Select Bay Camera", selection: $selection) {
                        ForEach(BayInputCases.allCases) {
                            Text($0.description)
                        }
                    }.onChange(of: $selection.wrappedValue) {
                        if !isApplyingRemoteState {
                            appModel.stateManager["bay"] = $selection.wrappedValue
                            bayNumber = $selection.wrappedValue.num - 1
                            broadcastControl(type: .bay, value: $selection.wrappedValue.rawValue)
                        }
                    }

                    .pickerStyle(.menu)
                    Button("Next") {
                        bayNumber += 1
                        if(bayNumber >= 7) {
                            bayNumber = 0
                        }
                        selection = BayInputCases.allCases[bayNumber]
                        appModel.stateManager["bay"] = BayInputCases.allCases[bayNumber]
                        broadcastControl(type: .bay, value: selection.rawValue)
                    }

                }.disabled(appState != .connected)

                if appModel.sharePlayManager.isSharePlayActive {
                    Spacer()
                    Text("SharePlay: \(appModel.sharePlayManager.participantCount) participant(s)")
                        .font(.caption)
                        .foregroundStyle(.secondary)
                }

                Spacer()
                Form{}.frame(height: 50)

            case .sharePlay:
                sharePlayView
            }

        }.ornament(visibility: .visible, attachmentAnchor: .scene(.init(x: 0.5, y: -0.05))) {
            HStack {
                Button("Config Screen") { uiPage = .config }
                Button("Controls") { uiPage = .controls }
                Button("SharePlay") { uiPage = .sharePlay }
            }
        }
        .padding(30).background(.accent.opacity(0.4))
        .onAppear {
            setupSharePlayCallbacks()
            appModel.sharePlayManager.listenForSessions()
        }
    }

    // MARK: - SharePlay View

    @ViewBuilder
    private var sharePlayView: some View {
        VStack(spacing: 16) {
            Spacer()

            Text("SharePlay")
                .font(.title2)

            if appModel.sharePlayManager.isSharePlayActive {
                Text("\(appModel.sharePlayManager.participantCount) participant(s)")
                    .font(.headline)
                    .foregroundStyle(.secondary)

                Button("End SharePlay") {
                    appModel.sharePlayManager.endSharePlay()
                }
                .tint(.red)
            } else {
                Text("Start a shared session so all participants control the same Omniverse scene together.")
                    .font(.caption)
                    .foregroundStyle(.secondary)
                    .multilineTextAlignment(.center)
                    .padding(.horizontal)

                Button("Start SharePlay") {
                    Task {
                        await appModel.sharePlayManager.startSharePlay()
                    }
                }
                .tint(.green)
            }

            if let error = appModel.sharePlayManager.sessionError {
                Text(error)
                    .font(.caption)
                    .foregroundStyle(.red)
            }

            Spacer()
        }
    }

    // MARK: - SharePlay Integration

    private func setupSharePlayCallbacks() {
        let manager = appModel.sharePlayManager

        // Remote participant changed bay or animation
        manager.onControlReceived = { [self] message in
            isApplyingRemoteState = true
            defer { isApplyingRemoteState = false }

            switch message.controlType {
            case .bay:
                if let bay = BayInputCases(rawValue: message.value) {
                    selection = bay
                    bayNumber = bay.num - 1
                    appModel.stateManager["bay"] = bay
                }
            case .animation:
                if let anim = AnimationCases(rawValue: message.value) {
                    appModel.stateManager["animationAction"] = anim
                    switch anim {
                    case .Play:
                        playButtonPressed = true
                    case .Pause, .Stop:
                        playButtonPressed = false
                    case .Rewind:
                        playButtonPressed = true
                    }
                }
            }
        }

        // Remote participant connected/disconnected
        manager.onConnectionReceived = { message in
            switch message.action {
            case .connect:
                if appState != .connected {
                    ipAddress = message.serverIP
                    Self.logger.info("SharePlay: received server IP \(message.serverIP)")
                }
            case .disconnect:
                Self.logger.info("SharePlay: remote participant disconnected")
            }
        }

        // Remote participant selected a prim
        manager.onSelectionReceived = { message in
            Self.logger.info("SharePlay: selection changed to \(message.selectedPrimPath ?? "none")")
            // Selection highlighting is handled in ImmersiveView;
            // the prim tap already goes through the shared server.
            // This callback is available for future local overlay sync.
        }
    }

    /// Broadcast a control state change to SharePlay participants.
    private func broadcastControl(type: SharePlayControlMessage.ControlType, value: String) {
        guard appModel.sharePlayManager.isSharePlayActive else { return }
        guard !isApplyingRemoteState else { return }
        appModel.sharePlayManager.sendControl(
            SharePlayControlMessage(controlType: type, value: value, senderID: appModel.sharePlayManager.localParticipantID)
        )
    }

    /// Broadcast a connection state change to SharePlay participants.
    private func broadcastConnection(action: SharePlayConnectionMessage.Action) {
        guard appModel.sharePlayManager.isSharePlayActive else { return }
        appModel.sharePlayManager.sendConnection(
            SharePlayConnectionMessage(action: action, serverIP: ipAddress, senderID: appModel.sharePlayManager.localParticipantID)
        )
    }
}

#Preview(windowStyle: .plain) {
    ContentView()
        .environment(AppModel(cxrSession: CloudXRSession(config: CloudXRKit.Config())))
        .background(.accent.opacity(0.4))
}
