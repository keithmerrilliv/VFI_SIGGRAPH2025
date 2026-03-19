// SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

@preconcurrency import CloudXRKit
import Foundation
import os.log

@MainActor public protocol OmniverseMessageListener: AnyObject {
    func onMessageReceived(message: Data)
}

public class OmniverseMessageDispatcher: @unchecked Sendable {
    private static let logger = Logger(
        subsystem: Bundle(for: OmniverseMessageDispatcher.self).bundleIdentifier!,
        category: String(describing: OmniverseMessageDispatcher.self)
    )

    private var channelListener: Task<Void, Never>?
    private var listeners = [OmniverseMessageListener]()

    /// Retained for reference but no longer starts a listener automatically.
    /// Use `messageChannel` instead — the legacy `session.serverMessageStream`
    /// is superseded by `MessageChannel.receivedMessageStream` in SDK 6.0.2.
    public var session: Session?

    /// Set the MessageChannel to listen on its receivedMessageStream.
    public var messageChannel: MessageChannel? {
        didSet {
            channelListener?.cancel()
            guard let channel = messageChannel else { return }
            self.channelListener = Task {
                for await message in channel.receivedMessageStream {
                    await MainActor.run {
                        self.listeners.forEach({ $0.onMessageReceived(message: message) })
                    }
                }
            }
            Self.logger.info("Dispatcher now listening on MessageChannel")
        }
    }

    public func attach(_ listener: OmniverseMessageListener) {
        // Remove any existing listener of the same type to prevent duplicates
        // when RealityKit creates multiple System instances (one per Scene).
        listeners.removeAll(where: { type(of: $0) == type(of: listener) })
        listeners.append(listener)
    }

    func detach(_ listener: OmniverseMessageListener) {
        if let index = listeners.firstIndex(where: { $0 === listener }) {
            listeners.remove(at: index)
        }
    }

}
