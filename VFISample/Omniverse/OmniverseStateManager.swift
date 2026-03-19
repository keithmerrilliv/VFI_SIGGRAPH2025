//
//  OmniverseStateManager.swift
//  CleansheetCloudXRClient
//
//  Created by Jen Borucki on 9/30/24.
//

import Foundation
@preconcurrency import CloudXRKit
import os.log

public class OmniverseStateManager {
    private static let logger = Logger(
        subsystem: Bundle(for: OmniverseStateManager.self).bundleIdentifier!,
        category: String(describing: OmniverseStateManager.self)
    )

    public struct OmniverseState {
        var currentState: (any OmniverseMessageProtocol)? = nil
        var desiredState: any OmniverseMessageProtocol
        var serverNotifiesCompletion: Bool
        var waitingForCompletion: Bool = false

        init(_ desiredState: any OmniverseMessageProtocol, serverNotifiesCompletion: Bool) {
            self.desiredState = desiredState
            self.serverNotifiesCompletion = serverNotifiesCompletion
        }
    }
    typealias StateDictionary = [String: OmniverseState]

    weak var session: Session?
    var messageChannel: MessageChannel?

    private var stateDict: StateDictionary = [
        "animationAction": .init(AnimationCases.Stop, serverNotifiesCompletion: false),
        "bay": .init(BayInputCases.Bay1, serverNotifiesCompletion: false)
    ]

    subscript(_ stateKey: String) -> (any OmniverseMessageProtocol)? {
        get {
            guard let state = stateDict[stateKey] else { return nil }
            return state.currentState
        }

        set {
            guard let newValue else { return }
            guard let state = stateDict[stateKey] else { return }
            if state.waitingForCompletion {
                Self.logger.error("Tried updating a state that is waiting for completion! \(stateKey)")
                return
            }
            stateDict[stateKey]?.desiredState = newValue
            sync()
        }
    }

    public func desiredState(_ key: String) -> any OmniverseMessageProtocol {
        guard let state = stateDict[key] else {
            fatalError("OmniverseStateManager: unknown state key '\(key)'")
        }
        return state.desiredState
    }

    public func isAwaitingCompletion(_ stateKey: String) -> Bool {
        guard let state = stateDict[stateKey] else { return false }
        return state.waitingForCompletion
    }

    /// Send data to the server via the MessageChannel.
    private func sendData(_ data: Data) {
        guard let channel = messageChannel else {
            Self.logger.warning("MessageChannel not ready — dropping message")
            return
        }
        _ = channel.sendServerMessage(data)
    }

    public func sync() {
        guard session != nil else { return }
        for (stateName, state) in stateDict {
            var newState = state
            if let currentState = state.currentState, currentState.isEqualTo(state.desiredState) {
                continue
            } else {
                sendData(encodeJSON(state.desiredState.encodable))
                Self.logger.info("Sending message to server: \(state.desiredState.encodable.message.description)")
                if state.serverNotifiesCompletion {
                    if state.currentState == nil {
                        newState.currentState = state.desiredState
                        newState.waitingForCompletion = false
                    } else {
                        newState.waitingForCompletion = true
                    }
                } else {
                    newState.currentState = state.desiredState
                }
                stateDict[stateName] = newState
            }
        }
    }

    public func send(_ message: any OmniverseMessageProtocol) {
        guard session != nil else { return }
        Self.logger.info("Sending message to server: \(message.encodable.message.description)")
        sendData(encodeJSON(message.encodable))
    }

    public func resync() {
        stateDict.forEach { stateDict[$0.0]?.currentState = nil }
        sync()
    }
}
