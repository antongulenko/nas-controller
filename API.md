# ESP Easy HTTP API

The [OpenAPI specification](./openapi.yaml) describes the ESP Easy Mega
endpoints used to control this NAS controller. Set its `host` server variable
to the controller's hostname or IP address before importing the spec into an
API client.

## Static dashboard

Serve [index.html](./index.html) as a static page. Configure the web server or
reverse proxy to expose the controller's `/control` and `/json` endpoints
beneath the app's same-origin path (by default
`/nas-controller-api/control` and `/nas-controller-api/json`).

The `APP_CONFIG` object near the top of `index.html` holds the API base path,
poll interval, timeout, and separate `bookmarks.public` and `bookmarks.local`
lists. Public links appear first; local links are for services on the home
network. Update these values to match your setup. On load, the page makes one
status query and does not send any button
commands. Polling every 15 seconds starts after the user first presses a
control or Refresh. Requests and their results appear in the activity panel.

## Actions

ESP Easy executes commands through `GET /control?cmd=...`:

| Action | Command | Effect |
| --- | --- | --- |
| Power | `Pulse,15,1,400` | Pulses the power relay on GPIO 15 for 400 ms. |
| Back | `event,back#pressed=1` | Raises the Back button event for the rules. |
| OK | `event,ok#pressed=1` | Raises the OK button event for the rules. |

For example, set the controller URL and press Back:

```sh
CONTROLLER_URL=http://nas-controller
curl --get --data-urlencode 'cmd=event,back#pressed=1' "$CONTROLLER_URL/control"
```

Use the same form with the Power or OK command from the table. URL-encode
commands as shown; in particular, `#` must not be sent as a URL fragment.
Back and OK trigger the events consumed by `rules3.txt`.

## NAS power state

`GET /json` returns ESP Easy's JSON status document. The NAS state is the first
value in the sensor task named `power`: `1` means on and `0` means off. To
print just the state with `curl` and `jq`:

```sh
curl -fsS "$CONTROLLER_URL/json" |
  jq -er '[.Sensors[]? | select(.TaskName == "power") | .TaskValues[0].Value]
    | if length != 1 then error("expected exactly one task named power")
      elif .[0] == 1 or .[0] == "1" then "on"
      elif .[0] == 0 or .[0] == "0" then "off"
      else error("unexpected power value: \(.[0])")
      end'
```

The [nas-controller-api.sh](./nas-controller-api.sh) helper wraps these
requests; its `status` action requires `jq`.
