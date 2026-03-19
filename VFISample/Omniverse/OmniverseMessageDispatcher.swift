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

    private var serverListener: Task<Void, Never>?
    private var channelListener: Task<Void, Never>?
    private var listeners = [OmniverseMessageListener]()

    public var session: Session? {
        didSet {
            serverListener?.cancel()
            self.serverListener = Task {
                await eventDecoder()
            }
        }
    }

    /// Set the MessageChannel to listen on its receivedMessageStream.
    /// Stops the legacy session listener to avoid duplicate delivery.
    public var messageChannel: MessageChannel? {
        didSet {
            channelListener?.cancel()
            guard let channel = messageChannel else { return }
            // Stop the legacy session listener — the channel supersedes it
            serverListener?.cancel()
            serverListener = nil
            self.channelListener = Task {
                for await message in channel.receivedMessageStream {
                    await MainActor.run {
                        listeners.forEach({ $0.onMessageReceived(message: message) })
                    }
                }
            }
            Self.logger.info("Dispatcher now listening on MessageChannel")
        }
    }

    public func attach(_ listener: OmniverseMessageListener) {
        detach(listener)
        listeners.append(listener)
    }

    func detach(_ listener: OmniverseMessageListener) {
        if let index = listeners.firstIndex(where: { $0 === listener }) {
            listeners.remove(at: index)
        }
    }

    private func eventDecoder() async {
        guard let session = self.session else {
            Self.logger.error("Message dispatcher's session is not valid, cannot listen to incoming messages from Omniverse!")
            return
        }
        for await message in session.serverMessageStream {
            await MainActor.run {
                listeners.forEach({ $0.onMessageReceived(message: message) })
            }
        }
    }
}
