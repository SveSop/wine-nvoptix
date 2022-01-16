#!/bin/bash

nvoptix_dir="$(dirname "$(readlink -fm "$0")")"
lib='x64'
wine='wine64'

if [ ! -f "$nvoptix_dir/$lib/nvoptix.dll.so" ]; then
    echo "nvoptix.dll.so not found in $nvoptix_dir/$lib" >&2
    exit 1
fi

winever=$($wine --version | grep wine)
if [ -z "$winever" ]; then
    echo "$wine:  Not a wine executable. Check your $wine." >&2
    exit 1
fi

assume=

function ask {
    echo "$1"
    if [ -z "$assume" ]; then
        read -r continue
    else
        continue=$assume
        echo "$continue"
    fi
}

POSITIONAL=()
while [[ $# -gt 0 ]]; do

    case $1 in
    -y)
        assume='y'
        shift
        ;;
    -n)
        assume='n'
        shift
        ;;
    *)
        POSITIONAL+=("$1")
        shift
        ;;
    esac
done
set -- "${POSITIONAL[@]}"

if [ -z "$WINEPREFIX" ]; then
    ask "WINEPREFIX is not set, continue? (y/N)"
    if [ "$continue" != "y" ] && [ "$continue" != "Y" ]; then
    exit 1
    fi
else
    if ! [ -f "$WINEPREFIX/system.reg" ]; then
        ask "WINEPREFIX does not point to an existing wine installation. Proceeding will create a new one, continue? (y/N)"
        if [ "$continue" != "y" ] && [ "$continue" != "Y" ]; then
        exit 1
        fi
    fi
fi

unix_sys_path=$($wine winepath -u 'C:\windows\system32' 2> /dev/null)

if [ -z "$unix_sys_path" ]; then
  echo 'Failed to resolve C:\windows\system32.' >&2
  exit 1
fi

function remove {
    echo "    Removing nvoptix... "
    local dll="$unix_sys_path/$1.dll"
    if [ -h "$dll" ]; then
        out=$(rm "$dll" 2>&1)
        if [ $? -ne 0 ]; then
            echo -e "$out"
            exit=2
        fi
    else
        echo -e "'$dll' is not a link or doesn't exist."
        exit=2
    fi
}

function create {
    echo "    Installing nvoptix... "
    ln -sf "$nvoptix_dir/$lib/$1.dll.so" "$unix_sys_path/$1.dll"
    if [ $? -ne 0 ]; then
        echo -e "Failed to create symlink"
        exit 1
    fi
}

case "$1" in
uninstall)
    fun=remove
    ;;
install)
    fun=create
    ;;
*)
    echo "Unrecognized option: $1"
    echo "Usage: $0 [install|uninstall]"
    exit 1
    ;;
esac

$fun nvoptix
exit $ret
