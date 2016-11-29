#!/usr/bin/env bash


[ ${BASH_VERSINFO:-0} -lt 3 ] && { echo "bash version >= 3 required: ${BASH_VERSINFO:-0}" 1>&2; exit 1; }


shopt -s extglob xpg_echo || { echo -e "\nshopt must support:\n  extglob\n  xpg_echo" 1>&2; exit 1; }


sp=$(echo -e "[ \t]")
tb=$(echo -e "\t")


if [ -n "$WINDIR" ]; then is_win=true; else is_win=false; fi
function prfx { local p="${1//\//\\/}" m; shift; for m in "$@"; do echo -e "$m" | sed -e "s/^/$p/" 1>&2; done; }
function rprt { echo '# info:  ' 1>&2 ; prfx '    ' "$@"; }
function warn { prfx 'warn:  ' "$@" 1>&2; }
function barf { warn "$@"; return 1; }
function die  { prfx 'die:  '  "$@" 1>&2; exit 1; }
function cdod { local d="$1" ; cd "$d" || die "Could not:" "  cd '$d'"; }
# function win_only  { $is_win || die "Only available on Windows/Cygwin."; }
# function unix_only { $is_win && die "Only available on Unix."; }
# win_only
# unix_only


declare -a default_commands=( \
  "f=2500" \
  "l=2500" \
  "h=1" \
  "r=2500" \
  "b=2500" \
  "h=1" \
  )


dry=false
help=false
out_dir="$PWD"
port=3000
protocol=http
today=$(date +'%Y%m%d')
url="127.0.0.1"
verbose=false
while getopts ':P:hp:u:vy' opt; do # {{{1
  case $opt in
    P)
      protocol="$OPTARG"
      ;;
    h)
      help=true
      ;;
    p)
      port="$OPTARG"
      ;;
    u)
      url="$OPTARG"
      ;;
    v)
      verbose=true
      ;;
    y)
      dry=true
      ;;
    *)
      o="$(eval "echo \"'\$$((OPTIND - 1))'\"")"
      die "Invalid option:  $o"
      ;;
  esac
done
shift $((OPTIND - 1))
$dry && verbose=true




if $help; then # {{{1
  cat <<EOHELP

Script name:

  $0

Description:

  TODO

Usage:

  $0 [-h] [-v] [-y]

Flags:

  -h
      Display this help message, then exit with success.
  -v
      Verbose mode.
  -y
      Dry run.  Automatically enables verbose mode.

EOHELP
  exit 0
fi




# functions # {{{1




# defaults # {{{1




# code # {{{1


declare -a commands=()
if [ ${#@} -gt 0 ]; then
  commands=("$@")
else
  commands=("${default_commands[@]}")
fi


cmd_url="$protocol://$url:$port/"
for c in "${commands[@]}"; do
  curl -q "$cmd_url?$c"
  sleep 2.5
done


# vim: ft=sh fdm=marker fdl=4 sw=2 sts=2 ts=8 et
