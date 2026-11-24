#!/usr/bin/env bash
set -euo pipefail

controller_url="${NAS_CONTROLLER_URL:-http://nas-controller}"
controller_url="${controller_url%/}"

usage() {
  printf 'Usage: NAS_CONTROLLER_URL=http://controller-host %s {power|back|ok|status}\n' "$0" >&2
  exit 2
}

[[ $# -eq 1 ]] || usage

case "$1" in
  power)
    command='Pulse,15,1,400'
    ;;
  back)
    command='event,back#pressed=1'
    ;;
  ok)
    command='event,ok#pressed=1'
    ;;
  status)
    curl -fsS --connect-timeout 5 "$controller_url/json" |
      jq -er '
        [.Sensors[]? | select(.TaskName == "power") | .TaskValues[0].Value] |
        if length != 1 then error("expected exactly one task named power")
        elif .[0] == 1 or .[0] == "1" then "on"
        elif .[0] == 0 or .[0] == "0" then "off"
        else error("unexpected power value: \(.[0])")
        end
      '
    exit
    ;;
  *)
    usage
    ;;
esac

curl -fsS --connect-timeout 5 --get \
  --data-urlencode "cmd=$command" \
  "$controller_url/control"
