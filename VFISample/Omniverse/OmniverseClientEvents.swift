//
//  OmniverseClientEvents.swift
//  CleansheetCloudXRClient
//
//  Created by Jen Borucki on 9/30/24.
//

import Foundation
@preconcurrency import CloudXRKit

private let jsonEncoder = JSONEncoder()

func encodeJSON(_ data: Encodable) -> Data {
    do {
        return try jsonEncoder.encode(data)
    } catch {
        fatalError("Failed to encode JSON: \(error)")
    }
}

public protocol EncodableInputEvent: Encodable, Equatable {
    associatedtype Parameter
    var message: Dictionary<String, Parameter> { get }
    var type: String { get }
}

/// Use ModelMessageProtocol to automate as much of the process as possible for sending messages to a CloudXR session
public protocol OmniverseMessageProtocol {
    /// The encodable object being sent to the session's sendServerMessage method after encoding
    var encodable: any EncodableInputEvent { get }
    
    func isEqualTo(_ other: OmniverseMessageProtocol?) -> Bool
}

extension OmniverseMessageProtocol where Self: Equatable {
    public func isEqualTo(_ other: OmniverseMessageProtocol?) -> Bool {
        guard let otherX = other as? Self else { return false }
        return self == otherX
    }
}

public struct BayInputEvent: EncodableInputEvent {
    public let message: Dictionary<String, String>
    public let type = "Bay"
    
    public init(_ bay: BayInputCases) {
        message = [
            "bay": bay.description
        ]
    }
    
}

public enum BayInputCases: String, CaseIterable, Identifiable, Equatable, OmniverseMessageProtocol {
    case Bay1
    case Bay2
    case Bay3
    case Bay4
    case Bay5
    case Bay6
    case Bay7

    public var id: Self {
        return self
    }
    
    public var description: String {
        switch self {
        case.Bay1:
            "bay_01"
        case.Bay2:
            "bay_02"
        case.Bay3:
            "bay_03"
        case.Bay4:
            "bay_04"
        case.Bay5:
            "bay_05"
        case.Bay6:
            "bay_06"
        case.Bay7:
            "bay_07"
        }
    }
    
    public var num: Int {
        switch self {
        case.Bay1:
            1
        case.Bay2:
            2
        case.Bay3:
            3
        case.Bay4:
            4
        case.Bay5:
            5
        case.Bay6:
            6
        case.Bay7:
            7
        }
    }
    
    public var encodable: any EncodableInputEvent { BayInputEvent(self) }
}

struct PrimTapInputEvent: MessageDictionary {
    public let message: [String: String]
    public let type = "PrimTap"

    public init(_ primPath: String) {
        message = [
            "PrimPath": primPath
        ]
    }
}

public struct AnimationInputEvent: EncodableInputEvent {
    public let message: Dictionary<String, String>
    public let type = "Animation"

    public init(_ animation: AnimationCases) {
        message = [
            "animationAction": animation.rawValue
        ]
    }
}

public struct DragInputEvent: EncodableInputEvent {
    public let message: Dictionary<String, String>
    public let type = "Drag"

    public init(deltaX: Float, deltaY: Float, deltaZ: Float, phase: String) {
        message = [
            "deltaX": String(deltaX),
            "deltaY": String(deltaY),
            "deltaZ": String(deltaZ),
            "phase": phase
        ]
    }
}

public struct ZoomInputEvent: EncodableInputEvent {
    public let message: Dictionary<String, String>
    public let type = "Zoom"

    public init(delta: Float, phase: String) {
        message = [
            "delta": String(delta),
            "phase": phase
        ]
    }
}

public enum AnimationCases: String, CaseIterable, OmniverseMessageProtocol {
    case Play
    case Stop
    case Rewind
    case Pause
    
    public var description: String {
        switch self {
        case .Play:
            "Play"
        case .Stop:
            "Stop"
        case .Rewind:
            "Rewind"
        case .Pause:
            "Pause"
        }
    }
    
    public var encodable: any EncodableInputEvent { AnimationInputEvent(self) }

}


