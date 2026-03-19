//
//  SharePlayManager.swift
//  VFISample
//
//  Manages GroupSession lifecycle and message relay for SharePlay.
//  Acts as a shared remote control layer — syncs UI control state
//  (bay, animation, selection) across participants. Does NOT sync
//  the CloudXR video stream (each headset has its own session).
//

import Foundation
import GroupActivities
import Combine
import os.log

@Observable @MainActor
final class SharePlayManager {
    private static let logger = Logger(
        subsystem: Bundle.main.bundleIdentifier ?? "SharePlayManager",
        category: "SharePlayManager"
    )

    // MARK: - Published State

    var isSharePlayActive: Bool = false
    var participantCount: Int = 0
    var sessionError: String?

    // MARK: - Callbacks

    /// Called when a remote participant sends a control change (bay or animation).
    var onControlReceived: ((SharePlayControlMessage) -> Void)?

    /// Called when a remote participant sends a connection message.
    var onConnectionReceived: ((SharePlayConnectionMessage) -> Void)?

    /// Called when a remote participant sends a selection change.
    var onSelectionReceived: ((SharePlaySelectionMessage) -> Void)?

    /// Called when a remote participant sends an immersive space action.
    var onImmersiveSpaceReceived: ((SharePlayImmersiveSpaceMessage) -> Void)?

    // MARK: - Private

    private var groupSession: GroupSession<SharedModelActivity>?
    private var messenger: GroupSessionMessenger?
    private var subscriptions = Set<AnyCancellable>()
    private var messageTasks: [Task<Void, Never>] = []

    /// Unique ID for this participant to avoid echo loops.
    let localParticipantID = UUID()

    // MARK: - Session Lifecycle

    /// Begin listening for incoming GroupSession activations.
    /// Call once at app launch.
    func listenForSessions() {
        Task {
            for await session in SharedModelActivity.sessions() {
                await self.configureSession(session)
            }
        }
    }

    /// Start a new SharePlay activity (initiator).
    func startSharePlay() async {
        let activity = SharedModelActivity()
        let result = await activity.prepareForActivation()

        switch result {
        case .activationPreferred:
            do {
                _ = try await activity.activate()
                Self.logger.info("SharePlay activity activated")
            } catch {
                Self.logger.error("Failed to activate SharePlay: \(error.localizedDescription)")
                sessionError = error.localizedDescription
            }
        case .activationDisabled:
            Self.logger.info("SharePlay activation disabled by system")
            sessionError = "SharePlay is not available"
        case .cancelled:
            Self.logger.info("SharePlay activation cancelled")
        @unknown default:
            break
        }
    }

    /// End the current SharePlay session.
    func endSharePlay() {
        groupSession?.end()
        cleanup()
    }

    // MARK: - Send Messages

    func sendControl(_ message: SharePlayControlMessage) {
        guard let messenger else { return }
        Task {
            do {
                try await messenger.send(message)
            } catch {
                Self.logger.error("Failed to send control: \(error.localizedDescription)")
            }
        }
    }

    func sendConnection(_ message: SharePlayConnectionMessage) {
        guard let messenger else { return }
        Task {
            do {
                try await messenger.send(message)
            } catch {
                Self.logger.error("Failed to send connection: \(error.localizedDescription)")
            }
        }
    }

    func sendSelection(_ message: SharePlaySelectionMessage) {
        guard let messenger else { return }
        Task {
            do {
                try await messenger.send(message)
            } catch {
                Self.logger.error("Failed to send selection: \(error.localizedDescription)")
            }
        }
    }

    func sendImmersiveSpace(_ message: SharePlayImmersiveSpaceMessage) {
        guard let messenger else { return }
        Task {
            do {
                try await messenger.send(message)
            } catch {
                Self.logger.error("Failed to send immersive space: \(error.localizedDescription)")
            }
        }
    }

    // MARK: - Private

    private func configureSession(_ session: GroupSession<SharedModelActivity>) async {
        cleanup()

        self.groupSession = session
        let messenger = GroupSessionMessenger(session: session)
        self.messenger = messenger

        // Track participant count
        session.$activeParticipants
            .sink { [weak self] participants in
                Task { @MainActor in
                    self?.participantCount = participants.count
                    Self.logger.info("SharePlay participants: \(participants.count)")
                }
            }
            .store(in: &subscriptions)

        // Track session state
        session.$state
            .sink { [weak self] state in
                Task { @MainActor in
                    switch state {
                    case .joined:
                        self?.isSharePlayActive = true
                        Self.logger.info("SharePlay session joined")
                    case .invalidated(let reason):
                        Self.logger.info("SharePlay session invalidated: \(String(describing: reason))")
                        self?.cleanup()
                    case .waiting:
                        Self.logger.info("SharePlay session waiting")
                    @unknown default:
                        break
                    }
                }
            }
            .store(in: &subscriptions)

        // Listen for incoming messages
        let controlTask = Task {
            for await (message, _) in messenger.messages(of: SharePlayControlMessage.self) {
                await MainActor.run {
                    guard message.senderID != self.localParticipantID else { return }
                    Self.logger.info("Received control: \(message.controlType.rawValue) = \(message.value)")
                    self.onControlReceived?(message)
                }
            }
        }

        let connectionTask = Task {
            for await (message, _) in messenger.messages(of: SharePlayConnectionMessage.self) {
                await MainActor.run {
                    guard message.senderID != self.localParticipantID else { return }
                    Self.logger.info("Received connection: \(message.action.rawValue)")
                    self.onConnectionReceived?(message)
                }
            }
        }

        let selectionTask = Task {
            for await (message, _) in messenger.messages(of: SharePlaySelectionMessage.self) {
                await MainActor.run {
                    guard message.senderID != self.localParticipantID else { return }
                    Self.logger.info("Received selection: \(message.selectedPrimPath ?? "none")")
                    self.onSelectionReceived?(message)
                }
            }
        }

        let immersiveTask = Task {
            for await (message, _) in messenger.messages(of: SharePlayImmersiveSpaceMessage.self) {
                await MainActor.run {
                    guard message.senderID != self.localParticipantID else { return }
                    Self.logger.info("Received immersive space: \(message.action.rawValue)")
                    self.onImmersiveSpaceReceived?(message)
                }
            }
        }

        messageTasks = [controlTask, connectionTask, selectionTask, immersiveTask]

        // Join the session
        session.join()
        isSharePlayActive = true
    }

    private func cleanup() {
        messageTasks.forEach { $0.cancel() }
        messageTasks.removeAll()
        subscriptions.removeAll()
        messenger = nil
        groupSession = nil
        isSharePlayActive = false
        participantCount = 0
        sessionError = nil
    }
}
