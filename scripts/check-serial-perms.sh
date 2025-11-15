#!/usr/bin/env bash
set -euo pipefail

PORT="${1:-${NEVILLE_SERIAL_PORT:-/dev/ttyUSB0}}"

if [[ ! -e "$PORT" ]]; then
  echo "Serial device '$PORT' was not found."
  printf "List detected USB serial devices with:\n  ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null\n"
  exit 1
fi

echo "Inspecting permissions for $PORT"
ls -l "$PORT"

device_group=$(stat -c %G "$PORT")
current_user=$(id -un)
user_groups=$(id -Gn)

echo
if grep -qw dialout <<< "$user_groups"; then
  echo "✅ User '$current_user' already belongs to the 'dialout' group."
else
  echo "⚠️  User '$current_user' is not in the 'dialout' group (current groups: $user_groups)."
  echo "   Add the user with:"
  echo "     sudo usermod -aG dialout $current_user"
  echo "   Then log out and back in (or reboot) so the new group membership takes effect."
fi

echo
if [[ "$device_group" != "dialout" ]]; then
  echo "ℹ️  Device group is '$device_group'. If this differs from 'dialout', adjust permissions or udev rules accordingly."
fi

echo
printf "Need a temporary workaround? Run (resets after unplug/reboot):\n  sudo chmod a+rw %s\n" "$PORT"
printf "To override the port used by Neville, export NEVILLE_SERIAL_PORT before building/running.\n"
