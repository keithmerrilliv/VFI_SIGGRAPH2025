//
//  SharedModelActivity.swift
//  VFISample
//
//  GroupActivity definition for SharePlay sessions.
//  Supports both Nearby (spatial) and Remote (FaceTime) modes.
//

import Foundation
import GroupActivities

struct SharedModelActivity: GroupActivity {
    static let activityIdentifier = "com.nvidia.ptcdemo.shared-review"

    var metadata: GroupActivityMetadata {
        var meta = GroupActivityMetadata()
        meta.title = "VFI Shared Review"
        meta.subtitle = "Review the factory digital twin together"
        meta.type = .generic
        meta.supportsContinuationOnTV = false
        return meta
    }
}
