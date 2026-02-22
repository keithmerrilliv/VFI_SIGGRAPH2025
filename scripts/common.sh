esc=$(printf '\033')

# Make sure Homebrew is on the path for xcbeautify, which parses logs and makes them human-readable,
# immediately calling out warnings and errors much more clearly.
export PATH="${PATH}:/opt/homebrew/bin"

# Use Xcode beta for now
export DEVELOPER_DIR=/Applications/Xcode-beta.app

echoColor() {
    if [ $# -ne 2 ]; then
        echo "Incorrect usage of echocolor - 2 args expected, got $#"
        echo "Usage: echoColor [color] \"text to display\""
        return
    fi
    PRINT_STRING="$2"

    COLOR_FG=0
    COLOR_BG=0

    case $1 in
    green)
        COLOR_FG=30
        COLOR_BG=42
        ;;
    cyan)
        COLOR_FG=30
        COLOR_BG=46
        ;;
    yellow)
        COLOR_FG=30
        COLOR_BG=43
        ;;
    red)
        COLOR_FG=30
        COLOR_BG=41
        ;;
    esac

    COLOR_BLOCK="${esc}[${COLOR_FG};${COLOR_BG}m"
    COLOR_RESET="${esc}[0m"

    echo -e "${COLOR_BLOCK}${PRINT_STRING}${COLOR_RESET}"
}

border() {
    echoColor cyan "XxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxX\r"
    echoColor cyan "XxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxXxX\r"
}

banner() {
    colorName="cyan"
    if [ $# -eq 2 ]; then
        colorName="$2"
    fi
    echoColor ${colorName} " \r"
    echoColor ${colorName} "`date` -- $1"
    echoColor ${colorName} " \r"
}

SECTION_TAG="\r${esc}[0K"

# sectionStart bulletName "Display text"
sectionStart() {
    if [ $# -ne 2 ]; then
        echo "Incorrect usage of sectionStart - 2 args expected, got $#"
        echo "Usage: sectionStart bulletName 'text to display'"
        return
    fi
    bullet="$1"
    text="$2"
    echoColor cyan "${SECTION_TAG}section_start:`date +%s`:${bullet}${SECTION_TAG}${text}"
}

sectionEnd() {
    if [ $# -ne 1 ]; then
        echo "Incorrect usage of sectionEnd - 1 arg expected, got $#"
        echo "Usage: sectionEnd bulletName"
        return
    fi
    bullet="$1"
    echoColor cyan "${SECTION_TAG}section_end:`date +%s`:${bullet}${SECTION_TAG}"
}



TEMP_DIR="test_output"
mkdir -p -- $TEMP_DIR
capture_mock_connection_screenshot() {
    MOCK_TEST_LOGFILE_PATH="$TEMP_DIR/mock_test_logs.log"
    SCREENSHOT_EXTENSION=".jpg"
    TIMESTEP=5
    TIMEOUT=120
    trap 'exit 0' SIGHUP
    waited=0
    while [ $waited -lt $TIMEOUT ] ; do
        echo "$waited seconds for simulator startup"
        ((waited += TIMESTEP))
        if (xcrun simctl list devices | grep -i "vision pro" | grep Booted) ; then
            xcrun simctl spawn booted log stream --level info | tee $MOCK_TEST_LOGFILE_PATH | while read -r line
            do
                if [[ $line == *"screenshot--"* ]]; then
                    sleep 1 # Give time for fade in animation
                    screenshot_filename=$(awk -F'screenshot--' '{print $2}' <<< $line)
                    echo "Taking screenshot for $screenshot_filename"
                    xcrun simctl io booted screenshot "$TEMP_DIR/$screenshot_filename$SCREENSHOT_EXTENSION"
                fi
            done
            break
        else
            sleep $TIMESTEP
        fi

    done
    echo "Done with screenshot function."
}



####
#### Find the right version of Xcode
####

REQUIRED_XCODE_VERSION=15.2
EXPLICIT_APP="/Applications/Xcode_${REQUIRED_XCODE_VERSION}.app"
BETA_APP="/Applications/Xcode-beta.app"
MAIN_APP="/Applications/Xcode.app"

expandedVersionString() {
	echo "$@" | awk -F. '{ printf("%d%03d%03d%03d\n", $1,$2,$3,$4); }'
}

appPlistVersion() {
	echo $(defaults read "${@}/Contents/Info.plist" CFBundleShortVersionString)
}

versionCheck() {
	APP="${@}"
	if [ -d "${APP}" ]; then
		xcodePlistVersion=$(appPlistVersion "${APP}")
		if [ $(expandedVersionString ${xcodePlistVersion}) -ge $(expandedVersionString ${REQUIRED_XCODE_VERSION}) ]; then
			return 0
		fi
	fi
	return 255
}

# 1. check for explicit Xcode_VERSION.app
# 2. check for Xcode_beta.app
# 3. check version of Xcode.app
#
if [ -d "${EXPLICIT_APP}" ]; then
	export DEVELOPER_DIR="${EXPLICIT_APP}"
elif versionCheck "${BETA_APP}"; then
	export DEVELOPER_DIR="${BETA_APP}"
elif versionCheck "${MAIN_APP}"; then
	export DEVELOPER_DIR="${MAIN_APP}"
else
	echo "Required Xcode version: ${REQUIRED_XCODE_VERSION}"
	exit 1
fi
####
####
####


# pipe output through helper in case beautify not installed..
pretty() {
	if [ -x "$bpath" ]; then
		$bpath
	else
		cat
	fi
}

checkBuild() {
    # PIPESTATUS[0] holds xcodebuild's status, PIPESTATUS[1] holds xcbeautify's status
    buildStatus="${PIPESTATUS[0]}"
    if [ $buildStatus != 0 ]; then
        banner "Build exited with code: $buildStatus"
        exit $buildStatus
    fi
}

