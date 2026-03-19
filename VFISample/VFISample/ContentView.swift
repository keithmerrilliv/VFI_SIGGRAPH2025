//
//  ContentView.swift
//  Cleansheet CloudXR Client
//
//  Created by Andrew Wilson on 8/28/24.
//

import SwiftUI
import os.log

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
                                await appModel.setupMessageChannel()
                                appState = .connected
                                uiPage = .controls
                                await openImmersiveSpace(id: immersiveTitle)
                            } catch {
                                Self.logger.error("Failed to connect: \(error.localizedDescription)")
                                appState = .disconnected
                            }
                            return
                        }

                        if appState == .connected {
                            appModel.teardownMessageChannel()
                            session.disconnect()
                            appState = .disconnected
                            return
                        }
                    }
                }.disabled(buttonDisabled).padding(25)
            case .controls:
                Spacer()
                Text("Animation Controls")

                HStack {
                    Button(playButtonPressed ? "Pause" : "Play") {

                        appModel.stateManager["animationAction"] = playButtonPressed ? AnimationCases.Pause : AnimationCases.Play
                        self.playButtonPressed.toggle()
                    }
                    Button("Stop") {
                        appModel.stateManager["animationAction"] = AnimationCases.Stop
                        self.playButtonPressed = false
                    }
                    Button("Rewind") {
                        appModel.stateManager["animationAction"] = AnimationCases.Rewind
                        self.playButtonPressed = true
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
                    }
                    Picker("Select Bay Camera", selection: $selection) {
                        ForEach(BayInputCases.allCases) {
                            Text($0.description)
                        }
                    }.onChange(of: $selection.wrappedValue) {
                        appModel.stateManager["bay"] = $selection.wrappedValue
                        bayNumber = $selection.wrappedValue.num - 1
                    }

                    .pickerStyle(.menu)
                    Button("Next") {
                        bayNumber += 1
                        if(bayNumber >= 7) {
                            bayNumber = 0
                        }
                        selection = BayInputCases.allCases[bayNumber]
                        appModel.stateManager["bay"] = BayInputCases.allCases[bayNumber]
                    }

                }.disabled(appState != .connected)
                Spacer()
                Form{}.frame(height: 50)
            }

        }.ornament(visibility: .visible, attachmentAnchor: .scene(.init(x: 0.5, y: -0.05))) {
            HStack {
                Button("Config Screen") { uiPage = .config }
                Button("Controls") {uiPage = .controls }
            }
        }.padding(30).background(.accent.opacity(0.4))
    }
}

#Preview(windowStyle: .plain) {
    ContentView()
        .environment(AppModel(cxrSession: CloudXRSession(config: CloudXRKit.Config())))
        .background(.accent.opacity(0.4))
}
