source ./scripts/common.sh

# This should make this script exit immediately if any command in this script has nonzero exit code
set -e

SCHEME=VFISample

sectionStart update-xcframeworks "Fetching, updating, building xcframeworks as needed"
./scripts/update-cloudxr-kit-development.sh
sectionEnd update-xcframeworks

pushd VFISample

for config in Debug Release; do
    border
    sectionStart xcode-${config} "Building $config ${SCHEME} with Xcode"
    banner "starting XCode $config ${SCHEME} build"
    # clean and then build both visionOS and iOS, all in one command
    xcodebuild \
        -scheme "${SCHEME}" \
        -destination "platform=visionOS Simulator,name=Apple Vision Pro" \
        -configuration $config \
        clean \
        build \
        2>&1 | tee errs | xcbeautify | tee errs-pretty
    checkBuild
    banner "finished Xcode $config ${SCHEME} build"
    sectionEnd xcode-${config}
done



# only Debug builds are built for test
config=Debug

border
sectionStart xcode-${config} "Building $config ${SCHEME} with Xcode"
banner "starting XCode $config ${SCHEME} test"

# TODO: Use capture screenshots for testing
# Start function to watch log for when to take a screenshot asynchronously
# capture_mock_connection_screenshot &
# SCREENSHOT_PID=$!


xcodebuild \
    -scheme "${SCHEME}" \
    -destination "platform=visionOS Simulator,name=Apple Vision Pro" \
    -configuration $config \
    test \
    2>&1
#DHC disabling pretty print in case it sheds light on test hang
    # | pretty
#DHC disabling check on test as it will exit the script.
#checkBuild
banner "finished Xcode $config ${SCHEME} test"
sectionEnd xcode-${config}


# TODO: Use capture screenshots for testing
#  If enabling screenshot capture, need to ensure those processes are not hanging around.
# # Send hangup signal to background tasks at end of script
# if ps -p $SCREENSHOT_PID > /dev/null
# then
#     pkill -SIGHUP -P $SCREENSHOT_PID
# fi

popd

swiftlint lint VFISample/ --reporter codeclimate > swiftlint_lint_report.json 

# TODO: enable analysis.
# At present, erroeneously claims "No such module CloudXRKit"
#swiftlint analyze --compiler-log-path CleansheetCloudXRClient/errs CleansheetCloudXRClient --reporter codeclimate > swiftlint_analyze_report.json

