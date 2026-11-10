# Calendar locale design

| Design | Status | Consumer |
| --- | --- | --- |
| [Shared calendar locale data](proposed/calendar_locale_design.md) | Implemented; eleven locale defaults, L1–L7 complete | [CWU chart time axis](../../../../Projects/smarthome_cwucontrol/frontend/docs/design/proposed/time_axis_design.md) |

The design separates immutable locale data from time arithmetic and display
formatting. Its implementation plan defines independent commit and validation
boundaries.
