//
//  SharePlayMessages.swift
//  VFISample
//
//  SharePlay message types for syncing control state across participants.
//  Since all participants connect to the same Omniverse server, these messages
//  sync the local UI/overlay state — not the 3D content itself.
//

import Foundation
import GroupActivities

// MARK: - Connection State

/// Syncs the server IP so all participants connect to the same Omniverse instance.
struct SharePlayConnectionMessage: Codable, Sendable {
    enum Action: String, Codable, Sendable {
        case connect
        case disconnect
    }

    let action: Action
    let serverIP: String
    let senderID: UUID
}

// MARK: - Control State

/// Syncs bay camera selection and animation controls across participants.
struct SharePlayControlMessage: Codable, Sendable {
    enum ControlType: String, Codable, Sendable {
        case bay
        case animation
    }

    let controlType: ControlType
    let value: String
    let senderID: UUID
}

// MARK: - Selection State

/// Syncs prim selection/highlight so all participants see the same selection overlay.
struct SharePlaySelectionMessage: Codable, Sendable {
    let selectedPrimPath: String?
    let senderID: UUID
}

// MARK: - Immersive Space State

/// Syncs immersive space enter/exit across participants.
struct SharePlayImmersiveSpaceMessage: Codable, Sendable {
    enum Action: String, Codable, Sendable {
        case enter
        case exit
    }

    let action: Action
    let senderID: UUID
}
