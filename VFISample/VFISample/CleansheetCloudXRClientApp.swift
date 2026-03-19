//
//  CleansheetCloudXRClientApp.swift
//  CleansheetCloudXRClient
//
//  Created by Andrew Wilson on 8/28/24.
//

import SwiftUI
import os.log
import GroupActivities

@preconcurrency import CloudXRKit

@main
struct CleansheetCloudXRClientApp: App {
    private static let logger = Logger()

    @State private var appModel = AppModel(
        cxrSession: CloudXRSession(
            config: CloudXRKit.Config()
        )
    )

    init() {
        #if targetEnvironment(simulator)
            Self.logger.info("Simulator Container URL: \(NSHomeDirectory())")
        #else
            // I don't think there's anything useful to log about on-device container, since we can't get it
        #endif
        CloudXRKit.registerSystems()
        OmniversePrimSystem.registerSystem()
    }

    var body: some Scene {
        WindowGroup {
            ContentView()
                .background(.accent.opacity(0.4))
        }
        .environment(appModel)
        .defaultSize(width: 400, height: 400).windowResizability(.contentMinSize)

        ImmersiveSpace(id: immersiveTitle) {
            StreamingView()
        }
        .environment(appModel)
    }
}
