## Version 0.2: XR Project Repo

Much improved. Important steps outside of copy-paste best version of code:
* Add the XCFrameworks
    * And then add them to "embed and sign"

TODO: Write up a full set of instructions.
  * Note: `scripts/build.sh` handles fetching deps and building — document this workflow.

Development flows and processes findings:

* CI / CD
  * Claim: Linting and unit test should be part of build
    * The way XCode does test essentially means you have to build the app, so it has the same source requirements as building
    * cannot run `swiftlint analyze` without build logs
    * unit tests and linting failing _is_ a failed build
    * **Status:** `scripts/build.sh` runs lint + unit tests as part of the build. Still needs a `.gitlab-ci.yml` to wire into GitLab CI.
  * Claim: Error out of builds at the first failure should be the default
     * Our builds are simple and sequential
     * Debugging a build failure that is buried halfway through the logs is a nightmare
     * Only build tools that aggregate and clearly report where errors occured should be allowed to not-terminate-early (e.g., xcodebuild but not our scripts)
     * **Status:** `build.sh` uses `set -e` (fail-fast). Done.


## Version 0.1: Private repo

from [Google Doc](https://docs.google.com/document/d/1wPLmABqE-ayNDnzwKGvqFsclqp3NNdjyr0MtPC1xACQ/edit#heading=h.sb8s9li6p5tn)

### Documentation Recommendations
Order of needs clearly documented:
* import CloudXRKit
    * Requires 3x frameworks: CloudXRKit, NVIDIAStreamKit, NVTelemetry (delivered as xcframeworks)
    * ~~Requires OrderedCollections from Swift Packages public repo~~ **Resolved:** OrderedCollections dependency removed.
    * MUST turn on “embed & sign” for all added frameworks (not on by default)
* Add CloudXR essentials to the main app
    * init() { CloudXRKit.registerSystems() }
    * ~~CloudXR sessionProvider and/or session~~ Simplified to `CloudXRKit.Session` directly.
* Set up root app file with (at minimum) 3 top level things:
    * Initial view
        * Typically a WindowGroup or similar
        * onAppear needs to beginIpdCheck(), which calls into next view
    * ImmersiveSpace / CompositorLayer
        * Compositor services “bounce”
        * Get IPD, other HMD properties not available in RealityKit
    * ImmersiveSpace / RealityView (s)
        * The scene(s) in which CloudXR will render streaming content
* ~~Copy over RealityKit content “Scene”~~ **Resolved:** Scenes are created programmatically from Omniverse server messages; no static USD assets needed.

### Issues to Address

* ~~Packaging, particularly “sub”-packaging~~ **Resolved:** All dependencies delivered as xcframeworks. No SPM, no OrderedCollections, no external protobuf (custom MessageProtocol.swift used instead).
* ~~Need to re-package the IPD compositor services bounce~~ **Resolved:** HMD/IPD checks handled inside CloudXRKit.xcframework.
* ~~Pain point: the activation of the compositor services check loop is not run in the simulator.~~ Simulator check exists in app init (`#if targetEnvironment(simulator)`); HMD check loop is inside CloudXRKit.
* ~~Pain point: Our API surface needs to have something more specific that Session and SessionProvider~~ **Resolved:** API simplified to single `CloudXRKit.Session`.
* ~~InfoSec: we need to be able to hide our ShaderGraph / shaders~~ **Resolved:** Shaders are baked into CloudXRKit.xcframework binary. No USD assets in the project.
* ~~Pain point: SessionProvider seems unneeded indirection~~ **Resolved:** SessionProvider removed; single `CloudXRKit.Session` used directly.
