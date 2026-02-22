//
//  CleansheetCloudXRClientTests.swift
//  CleansheetCloudXRClientTests
//
//  Created by Andrew Wilson on 8/28/24.
//

import XCTest
@testable import VFISample
import RealityKit
import simd

// MARK: - Bay Input Tests

class BayInputTests: XCTestCase {

    func testBayInputCasesCount() {
        XCTAssertEqual(BayInputCases.allCases.count, 7)
    }

    func testBayDescriptions() {
        let expected: [(BayInputCases, String)] = [
            (.Bay1, "bay_01"), (.Bay2, "bay_02"), (.Bay3, "bay_03"),
            (.Bay4, "bay_04"), (.Bay5, "bay_05"), (.Bay6, "bay_06"),
            (.Bay7, "bay_07")
        ]
        for (bay, desc) in expected {
            XCTAssertEqual(bay.description, desc)
        }
    }

    func testBayNums() {
        for (index, bay) in BayInputCases.allCases.enumerated() {
            XCTAssertEqual(bay.num, index + 1)
        }
    }

    func testBayIdentifiable() {
        for bay in BayInputCases.allCases {
            XCTAssertEqual(bay.id, bay)
        }
    }

    func testBayInputEventMessage() {
        let event = BayInputEvent(.Bay3)
        XCTAssertEqual(event.type, "Bay")
        XCTAssertEqual(event.message["bay"], "bay_03")
        XCTAssertEqual(event.message.count, 1)
    }

    func testBayInputEventEncoding() {
        let event = BayInputEvent(.Bay1)
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "Bay")
        let message = json?["message"] as? [String: String]
        XCTAssertEqual(message?["bay"], "bay_01")
    }

    func testBayEncodableProperty() {
        let bay = BayInputCases.Bay5
        let encodable = bay.encodable
        XCTAssertEqual(encodable.type, "Bay")
        XCTAssertEqual(encodable.message["bay"] as? String, "bay_05")
    }

    func testAllBaysEncodeCorrectly() {
        for bay in BayInputCases.allCases {
            let data = encodeJSON(BayInputEvent(bay))
            let decoded = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
            let message = decoded?["message"] as? [String: String]
            XCTAssertEqual(message?["bay"], bay.description, "Bay \(bay) encoding mismatch")
        }
    }
}

// MARK: - Animation Input Tests

class AnimationInputTests: XCTestCase {

    func testAnimationCasesCount() {
        XCTAssertEqual(AnimationCases.allCases.count, 4)
    }

    func testAnimationRawValues() {
        XCTAssertEqual(AnimationCases.Play.rawValue, "Play")
        XCTAssertEqual(AnimationCases.Stop.rawValue, "Stop")
        XCTAssertEqual(AnimationCases.Rewind.rawValue, "Rewind")
        XCTAssertEqual(AnimationCases.Pause.rawValue, "Pause")
    }

    func testAnimationDescriptions() {
        for animation in AnimationCases.allCases {
            XCTAssertEqual(animation.description, animation.rawValue)
        }
    }

    func testAnimationInputEventMessage() {
        let event = AnimationInputEvent(.Play)
        XCTAssertEqual(event.type, "Animation")
        XCTAssertEqual(event.message["animationAction"], "Play")
        XCTAssertEqual(event.message.count, 1)
    }

    func testAnimationInputEventEncoding() {
        let event = AnimationInputEvent(.Rewind)
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "Animation")
        let message = json?["message"] as? [String: String]
        XCTAssertEqual(message?["animationAction"], "Rewind")
    }

    func testAnimationEncodableProperty() {
        let encodable = AnimationCases.Stop.encodable
        XCTAssertEqual(encodable.type, "Animation")
        XCTAssertEqual(encodable.message["animationAction"] as? String, "Stop")
    }

    func testAllAnimationsEncodeCorrectly() {
        for animation in AnimationCases.allCases {
            let data = encodeJSON(AnimationInputEvent(animation))
            let decoded = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
            let message = decoded?["message"] as? [String: String]
            XCTAssertEqual(message?["animationAction"], animation.rawValue, "Animation \(animation) encoding mismatch")
        }
    }
}

// MARK: - PrimTapInputEvent Tests

class PrimTapInputEventTests: XCTestCase {

    func testPrimTapInputEventMessage() {
        let event = PrimTapInputEvent("/World/DGX_Tray")
        XCTAssertEqual(event.type, "PrimTap")
        XCTAssertEqual(event.message["PrimPath"], "/World/DGX_Tray")
        XCTAssertEqual(event.message.count, 1)
    }

    func testPrimTapInputEventEncoding() {
        let event = PrimTapInputEvent("/World/Test/Path")
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "PrimTap")
        let message = json?["message"] as? [String: String]
        XCTAssertEqual(message?["PrimPath"], "/World/Test/Path")
    }
}

// MARK: - RequestPrimInfoInputEvent Tests

class RequestPrimInfoInputEventTests: XCTestCase {

    func testMessage() {
        let event = RequestPrimInfoInputEvent("/World/DGX_Tray")
        XCTAssertEqual(event.type, "initial_prim_path")
        XCTAssertEqual(event.message["PrimPath"], "/World/DGX_Tray")
        XCTAssertEqual(event.message.count, 1)
    }

    func testEncoding() {
        let event = RequestPrimInfoInputEvent("/some/prim/path")
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "initial_prim_path")
        let message = json?["message"] as? [String: String]
        XCTAssertEqual(message?["PrimPath"], "/some/prim/path")
    }
}

// MARK: - SetPrimTransformationInputEvent Tests

class SetPrimTransformationInputEventTests: XCTestCase {

    func testIdentityTransformEvent() {
        let event = SetPrimTransformationInputEvent("/World/Test", transformation: .identity)
        XCTAssertEqual(event.type, "write_prim_transformation_extension")
        XCTAssertEqual(event.message["PrimPath"], "/World/Test")

        let values = event.message["Value"]!.split(separator: ",").map { Float($0)! }
        // 4 columns x 3 rows (x, y, z only — w row is omitted)
        XCTAssertEqual(values.count, 12)

        // Column 0: (1, 0, 0)
        XCTAssertEqual(values[0], 1.0, accuracy: 0.01)
        XCTAssertEqual(values[1], 0.0, accuracy: 0.01)
        XCTAssertEqual(values[2], 0.0, accuracy: 0.01)
        // Column 1: (0, 1, 0)
        XCTAssertEqual(values[3], 0.0, accuracy: 0.01)
        XCTAssertEqual(values[4], 1.0, accuracy: 0.01)
        XCTAssertEqual(values[5], 0.0, accuracy: 0.01)
        // Column 2: (0, 0, 1)
        XCTAssertEqual(values[6], 0.0, accuracy: 0.01)
        XCTAssertEqual(values[7], 0.0, accuracy: 0.01)
        XCTAssertEqual(values[8], 1.0, accuracy: 0.01)
        // Column 3 (translation): (0, 0, 0)
        XCTAssertEqual(values[9], 0.0, accuracy: 0.01)
        XCTAssertEqual(values[10], 0.0, accuracy: 0.01)
        XCTAssertEqual(values[11], 0.0, accuracy: 0.01)
    }

    func testTranslationTransformEvent() {
        var transform = Transform.identity
        transform.translation = simd_float3(1.5, 2.5, 3.5)
        let event = SetPrimTransformationInputEvent("/World/Moved", transformation: transform)

        let values = event.message["Value"]!.split(separator: ",").map { Float($0)! }
        // Translation is in column 3 (indices 9, 10, 11)
        XCTAssertEqual(values[9], 1.5, accuracy: 0.01)
        XCTAssertEqual(values[10], 2.5, accuracy: 0.01)
        XCTAssertEqual(values[11], 3.5, accuracy: 0.01)
    }

    func testRoundingBehavior() {
        // Values are rounded: (x * 100).rounded() / 100
        var transform = Transform.identity
        transform.translation = simd_float3(1.555, 2.444, 3.999)
        let event = SetPrimTransformationInputEvent("/World/Rounded", transformation: transform)

        let values = event.message["Value"]!.split(separator: ",").map { Float($0)! }
        XCTAssertEqual(values[9], 1.56, accuracy: 0.01)
        XCTAssertEqual(values[10], 2.44, accuracy: 0.01)
        XCTAssertEqual(values[11], 4.0, accuracy: 0.01)
    }
}

// MARK: - PrimShapeInfoStruct Tests

class PrimShapeInfoStructTests: XCTestCase {

    func testDefaultInitialization() {
        let info = PrimShapeInfoStruct()
        XCTAssertEqual(info.boundingBoxSize, simd_float3(0, 0, 0))
        XCTAssertEqual(info.boundingBoxCenter, simd_float3(0, 0, 0))
        XCTAssertEqual(info.worldPosition, simd_float3(0, 0, 0))
    }

    func testCustomInitialization() {
        let size = simd_float3(1.0, 2.0, 3.0)
        let center = simd_float3(0.5, 1.0, 1.5)
        let position = simd_float3(10.0, 20.0, 30.0)
        let info = PrimShapeInfoStruct(
            boundingBoxSize: size,
            boundingBoxCenter: center,
            worldPosition: position
        )
        XCTAssertEqual(info.boundingBoxSize, size)
        XCTAssertEqual(info.boundingBoxCenter, center)
        XCTAssertEqual(info.worldPosition, position)
    }
}

// MARK: - OmniversePrimSystem Static Methods Tests

class OmniversePrimSystemTests: XCTestCase {

    @MainActor
    func testProcessBoxShapeInfoValid() {
        let input = ["/World/DGX_Tray, 1.0, 2.0, 3.0, 0.5, 1.0, 1.5, 10.0, 20.0, 30.0"]
        let (primPath, shapeInfo) = OmniversePrimSystem.processBoxShapeInfo(boundingBoxString: input)

        XCTAssertEqual(primPath, "/World/DGX_Tray")
        XCTAssertEqual(shapeInfo.boundingBoxSize, simd_float3(1.0, 2.0, 3.0))
        XCTAssertEqual(shapeInfo.boundingBoxCenter, simd_float3(0.5, 1.0, 1.5))
        XCTAssertEqual(shapeInfo.worldPosition, simd_float3(10.0, 20.0, 30.0))
    }

    @MainActor
    func testProcessBoxShapeInfoLongPrimPath() {
        let input = ["/World/ProdLine/assembly/V02/Machine, 5.0, 6.0, 7.0, 2.5, 3.0, 3.5, 100.0, 200.0, 300.0"]
        let (primPath, shapeInfo) = OmniversePrimSystem.processBoxShapeInfo(boundingBoxString: input)

        XCTAssertEqual(primPath, "/World/ProdLine/assembly/V02/Machine")
        XCTAssertEqual(shapeInfo.boundingBoxSize, simd_float3(5.0, 6.0, 7.0))
        XCTAssertEqual(shapeInfo.boundingBoxCenter, simd_float3(2.5, 3.0, 3.5))
        XCTAssertEqual(shapeInfo.worldPosition, simd_float3(100.0, 200.0, 300.0))
    }

    @MainActor
    func testFindParentReturnsLongestMatch() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/Car": Entity(),
            "/World/Car/Door": Entity(),
            "/World/Car/Door/Handle": Entity()
        ]

        let parent = OmniversePrimSystem.findParent(
            primPath: "/World/Car/Door/Handle/Screw",
            objectNameAnchorEntities: entities
        )
        XCTAssertEqual(parent, "/World/Car/Door/Handle")
    }

    @MainActor
    func testFindParentReturnsImmediateParent() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/Car": Entity()
        ]

        let parent = OmniversePrimSystem.findParent(
            primPath: "/World/Car",
            objectNameAnchorEntities: entities
        )
        XCTAssertEqual(parent, "/World")
    }

    @MainActor
    func testFindParentReturnsEmptyForRoot() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/Car": Entity()
        ]

        let parent = OmniversePrimSystem.findParent(
            primPath: "/World",
            objectNameAnchorEntities: entities
        )
        XCTAssertEqual(parent, "")
    }

    @MainActor
    func testFindParentReturnsEmptyWhenNoMatch() {
        let entities: [String: Entity] = [
            "/World/Car": Entity()
        ]

        let parent = OmniversePrimSystem.findParent(
            primPath: "/Other/Path",
            objectNameAnchorEntities: entities
        )
        XCTAssertEqual(parent, "")
    }

    @MainActor
    func testFindChildrenReturnsDirectChildrenOnly() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/Car": Entity(),
            "/World/Car/Door": Entity(),
            "/World/Car/Door/Handle": Entity(),
            "/World/Tree": Entity()
        ]

        let children = OmniversePrimSystem.findChildren(
            primPath: "/World",
            objectNameAnchorEntities: entities
        )
        // /World/Car and /World/Tree are direct children
        // /World/Car/Door is a grandchild (child of /World/Car), not a direct child
        XCTAssertEqual(Set(children), Set(["/World/Car", "/World/Tree"]))
    }

    @MainActor
    func testFindChildrenReturnsEmptyForLeaf() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/Car": Entity()
        ]

        let children = OmniversePrimSystem.findChildren(
            primPath: "/World/Car",
            objectNameAnchorEntities: entities
        )
        XCTAssertEqual(children, [])
    }

    @MainActor
    func testFindChildrenDoesNotIncludeSelf() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/Car": Entity()
        ]

        let children = OmniversePrimSystem.findChildren(
            primPath: "/World",
            objectNameAnchorEntities: entities
        )
        XCTAssertFalse(children.contains("/World"))
    }

    @MainActor
    func testFindChildrenWithMultipleLevels() {
        let entities: [String: Entity] = [
            "/World": Entity(),
            "/World/A": Entity(),
            "/World/A/B": Entity(),
            "/World/A/B/C": Entity(),
            "/World/X": Entity(),
            "/World/X/Y": Entity()
        ]

        // Direct children of /World/A should be /World/A/B only
        let children = OmniversePrimSystem.findChildren(
            primPath: "/World/A",
            objectNameAnchorEntities: entities
        )
        XCTAssertEqual(children, ["/World/A/B"])
    }
}

// MARK: - OmniversePrimComponent Tests

class OmniversePrimComponentTests: XCTestCase {

    @MainActor
    func testInitialState() {
        let component = OmniversePrimComponent(primPath: "/World/Test")
        XCTAssertEqual(component.primPath, "/World/Test")
        XCTAssertFalse(component.representativeBoxCreated)
        XCTAssertFalse(component.shapeInfoRequested)
        XCTAssertEqual(component.metadataName, "")
        XCTAssertEqual(component.metadataDescription, "")
        XCTAssertFalse(component.updateText)
        XCTAssertEqual(component.remoteOmniversePrimTransform, .identity)
        XCTAssertEqual(component.localOmniversePrimTransform, .identity)
        XCTAssertEqual(component.latestCameraTransform, .identity)
    }

    @MainActor
    func testShapeInfoDefaultsToZero() {
        let component = OmniversePrimComponent(primPath: "/World/Test")
        XCTAssertEqual(component.shapeInfo.boundingBoxSize, simd_float3(0, 0, 0))
        XCTAssertEqual(component.shapeInfo.boundingBoxCenter, simd_float3(0, 0, 0))
        XCTAssertEqual(component.shapeInfo.worldPosition, simd_float3(0, 0, 0))
    }
}

// MARK: - BaseMessageDictionary Tests

class BaseMessageDictionaryTests: XCTestCase {

    func testDefaultValues() {
        let msg = BaseMessageDictionary()
        XCTAssertEqual(msg.message, [:])
        XCTAssertEqual(msg.type, "")
    }

    func testEquality() {
        XCTAssertEqual(BaseMessageDictionary(), BaseMessageDictionary())
    }

    func testEncoding() {
        let msg = BaseMessageDictionary()
        let data = encodeJSON(msg)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "")
    }
}

// MARK: - MessageProtocol Equality Tests

class MessageProtocolEqualityTests: XCTestCase {

    func testSameValueEquality() {
        XCTAssertTrue(BayInputCases.Bay1.isEqualTo(BayInputCases.Bay1))
        XCTAssertTrue(AnimationCases.Play.isEqualTo(AnimationCases.Play))
    }

    func testDifferentValueInequality() {
        XCTAssertFalse(BayInputCases.Bay1.isEqualTo(BayInputCases.Bay2))
        XCTAssertFalse(AnimationCases.Play.isEqualTo(AnimationCases.Stop))
    }

    func testNilInequality() {
        XCTAssertFalse(BayInputCases.Bay1.isEqualTo(nil))
        XCTAssertFalse(AnimationCases.Play.isEqualTo(nil))
    }

    func testCrossTypeInequality() {
        let bay: OmniverseMessageProtocol = BayInputCases.Bay1
        let animation: OmniverseMessageProtocol = AnimationCases.Play
        XCTAssertFalse(bay.isEqualTo(animation))
        XCTAssertFalse(animation.isEqualTo(bay))
    }
}

// MARK: - JSON Encoding Tests

class JSONEncodingTests: XCTestCase {

    func testEncodeJSONProducesValidJSON() {
        let event = BayInputEvent(.Bay1)
        let data = encodeJSON(event)
        XCTAssertFalse(data.isEmpty)
        XCTAssertNotNil(try? JSONSerialization.jsonObject(with: data))
    }

    func testEncodeJSONRoundTrip() {
        let event = AnimationInputEvent(.Play)
        let data = encodeJSON(event)
        let decoded = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertEqual(decoded?["type"] as? String, "Animation")
        let message = decoded?["message"] as? [String: String]
        XCTAssertEqual(message?["animationAction"], "Play")
    }
}

// MARK: - DragInputEvent Tests

class DragInputEventTests: XCTestCase {

    func testDragInputEventMessage() {
        let event = DragInputEvent(deltaX: 0.5, deltaY: -0.3, deltaZ: 0.0, phase: "changed")
        XCTAssertEqual(event.type, "Drag")
        XCTAssertEqual(event.message["deltaX"], "0.5")
        XCTAssertEqual(event.message["deltaY"], "-0.3")
        XCTAssertEqual(event.message["deltaZ"], "0.0")
        XCTAssertEqual(event.message["phase"], "changed")
        XCTAssertEqual(event.message.count, 4)
    }

    func testDragInputEventEncoding() {
        let event = DragInputEvent(deltaX: 0.1, deltaY: -0.2, deltaZ: 0.0, phase: "changed")
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "Drag")
        let message = json?["message"] as? [String: String]
        XCTAssertNotNil(message)
        XCTAssertEqual(message?["deltaX"], "0.1")
        XCTAssertEqual(message?["deltaY"], "-0.2")
        XCTAssertEqual(message?["deltaZ"], "0.0")
        XCTAssertEqual(message?["phase"], "changed")
    }

    func testDragInputEventZeroDeltas() {
        let event = DragInputEvent(deltaX: 0, deltaY: 0, deltaZ: 0, phase: "ended")
        XCTAssertEqual(event.message["deltaX"], "0.0")
        XCTAssertEqual(event.message["deltaY"], "0.0")
        XCTAssertEqual(event.message["deltaZ"], "0.0")
        XCTAssertEqual(event.message["phase"], "ended")
    }

    func testDragInputEventSmallValues() {
        let event = DragInputEvent(deltaX: 0.0005, deltaY: -0.0005, deltaZ: 0, phase: "changed")
        // Verify small values encode to parseable strings
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        let message = json?["message"] as? [String: String]
        XCTAssertNotNil(message?["deltaX"])
        XCTAssertNotNil(message?["deltaY"])
        // Server uses float() which handles scientific notation
        XCTAssertNotNil(Float(message?["deltaX"] ?? ""))
        XCTAssertNotNil(Float(message?["deltaY"] ?? ""))
    }

    func testDragInputEventEquality() {
        let a = DragInputEvent(deltaX: 1.0, deltaY: 2.0, deltaZ: 0.0, phase: "changed")
        let b = DragInputEvent(deltaX: 1.0, deltaY: 2.0, deltaZ: 0.0, phase: "changed")
        let c = DragInputEvent(deltaX: 3.0, deltaY: 2.0, deltaZ: 0.0, phase: "changed")
        XCTAssertEqual(a, b)
        XCTAssertNotEqual(a, c)
    }

    func testDragInputEventPhases() {
        for phase in ["began", "changed", "ended"] {
            let event = DragInputEvent(deltaX: 0, deltaY: 0, deltaZ: 0, phase: phase)
            XCTAssertEqual(event.message["phase"], phase)
        }
    }
}

// MARK: - ZoomInputEvent Tests

class ZoomInputEventTests: XCTestCase {

    func testZoomInputEventMessage() {
        let event = ZoomInputEvent(delta: 0.5, phase: "changed")
        XCTAssertEqual(event.type, "Zoom")
        XCTAssertEqual(event.message["delta"], "0.5")
        XCTAssertEqual(event.message["phase"], "changed")
        XCTAssertEqual(event.message.count, 2)
    }

    func testZoomInputEventEncoding() {
        let event = ZoomInputEvent(delta: -0.3, phase: "changed")
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertNotNil(json)
        XCTAssertEqual(json?["type"] as? String, "Zoom")
        let message = json?["message"] as? [String: String]
        XCTAssertNotNil(message)
        XCTAssertEqual(message?["delta"], "-0.3")
        XCTAssertEqual(message?["phase"], "changed")
    }

    func testZoomInputEventZeroDelta() {
        let event = ZoomInputEvent(delta: 0, phase: "ended")
        XCTAssertEqual(event.message["delta"], "0.0")
        XCTAssertEqual(event.message["phase"], "ended")
    }

    func testZoomInputEventNegativeDelta() {
        let event = ZoomInputEvent(delta: -1.5, phase: "changed")
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        let message = json?["message"] as? [String: String]
        XCTAssertEqual(message?["delta"], "-1.5")
    }

    func testZoomInputEventEquality() {
        let a = ZoomInputEvent(delta: 0.5, phase: "changed")
        let b = ZoomInputEvent(delta: 0.5, phase: "changed")
        let c = ZoomInputEvent(delta: -0.5, phase: "changed")
        XCTAssertEqual(a, b)
        XCTAssertNotEqual(a, c)
    }

    func testZoomInputEventPhases() {
        for phase in ["began", "changed", "ended"] {
            let event = ZoomInputEvent(delta: 0.1, phase: phase)
            XCTAssertEqual(event.message["phase"], phase)
        }
    }
}

// MARK: - Gesture Event JSON Round-Trip Tests

class GestureEventJSONRoundTripTests: XCTestCase {

    /// Verify that Drag JSON can be parsed back exactly as the server expects.
    func testDragRoundTripServerFormat() {
        let event = DragInputEvent(deltaX: 0.015, deltaY: -0.008, deltaZ: 0.0, phase: "changed")
        let data = encodeJSON(event)
        // The server receives the full JSON and routes by "type", then
        // parse_message extracts the inner "message" dict.
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertEqual(json?["type"] as? String, "Drag")

        let message = json?["message"] as? [String: String]
        XCTAssertNotNil(message)

        // Server does: float(message.get('deltaX', '0.0'))
        let dx = Float(message?["deltaX"] ?? "0.0")
        let dy = Float(message?["deltaY"] ?? "0.0")
        let dz = Float(message?["deltaZ"] ?? "0.0")
        XCTAssertNotNil(dx)
        XCTAssertNotNil(dy)
        XCTAssertNotNil(dz)
        XCTAssertEqual(dx!, 0.015, accuracy: 0.0001)
        XCTAssertEqual(dy!, -0.008, accuracy: 0.0001)
        XCTAssertEqual(dz!, 0.0, accuracy: 0.0001)
    }

    /// Verify that Zoom JSON can be parsed back exactly as the server expects.
    func testZoomRoundTripServerFormat() {
        let event = ZoomInputEvent(delta: 0.05, phase: "changed")
        let data = encodeJSON(event)
        let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
        XCTAssertEqual(json?["type"] as? String, "Zoom")

        let message = json?["message"] as? [String: String]
        let delta = Float(message?["delta"] ?? "0.0")
        XCTAssertNotNil(delta)
        XCTAssertEqual(delta!, 0.05, accuracy: 0.0001)
    }
}

// MARK: - OmniverseStateManager Tests

class OmniverseStateManagerTests: XCTestCase {

    func testDefaultDesiredStates() {
        let manager = OmniverseStateManager()

        XCTAssertTrue(manager.desiredState("animationAction").isEqualTo(AnimationCases.Stop))
        XCTAssertTrue(manager.desiredState("bay").isEqualTo(BayInputCases.Bay1))
    }

    func testCurrentStateIsNilInitially() {
        let manager = OmniverseStateManager()
        XCTAssertNil(manager["animationAction"])
        XCTAssertNil(manager["bay"])
    }

    func testSubscriptReturnsNilForUnknownKey() {
        let manager = OmniverseStateManager()
        XCTAssertNil(manager["nonExistentKey"])
    }

    func testIsAwaitingCompletionDefaultsFalse() {
        let manager = OmniverseStateManager()
        XCTAssertFalse(manager.isAwaitingCompletion("animationAction"))
        XCTAssertFalse(manager.isAwaitingCompletion("bay"))
    }

    func testIsAwaitingCompletionForUnknownKey() {
        let manager = OmniverseStateManager()
        XCTAssertFalse(manager.isAwaitingCompletion("nonExistent"))
    }

    func testSubscriptSetIgnoresNil() {
        let manager = OmniverseStateManager()
        manager["animationAction"] = nil
        // desiredState should remain unchanged
        XCTAssertTrue(manager.desiredState("animationAction").isEqualTo(AnimationCases.Stop))
    }

    func testSubscriptSetIgnoresNewKeys() {
        let manager = OmniverseStateManager()
        manager["newKey"] = AnimationCases.Play
        XCTAssertNil(manager["newKey"])
    }

    func testSubscriptSetUpdatesDesiredState() {
        let manager = OmniverseStateManager()
        // Without a session, sync() returns early but desiredState is still updated
        manager["animationAction"] = AnimationCases.Play
        XCTAssertTrue(manager.desiredState("animationAction").isEqualTo(AnimationCases.Play))
    }

    func testSubscriptSetUpdatesBayDesiredState() {
        let manager = OmniverseStateManager()
        manager["bay"] = BayInputCases.Bay5
        XCTAssertTrue(manager.desiredState("bay").isEqualTo(BayInputCases.Bay5))
    }

    func testResyncClearsCurrentState() {
        let manager = OmniverseStateManager()
        // Set desired state (without session, currentState won't sync but desiredState updates)
        manager["animationAction"] = AnimationCases.Play
        manager.resync()
        // After resync, currentState should be nil
        XCTAssertNil(manager["animationAction"])
    }
}
