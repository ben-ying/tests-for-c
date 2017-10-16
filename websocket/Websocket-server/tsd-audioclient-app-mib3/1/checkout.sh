#!/bin/bash

on_exit()
{
     if [[ -n "$_sandbox" ]] ; then
          if [[ $_keep_sandbox = 0 ]] ; then
                rm -rf "$_sandbox"
          else
                echo "Keeping sandbox in $_sandbox" >&2
          fi
     fi
}

run()
{
     "$@"
}

run_script()
{
    local ret=0 trace=""
    if [[ $_verbose -ge 3 ]] ; then trace="-x" ; fi

    echo "### START: `date`"
    run /bin/bash $trace -- ../script 
    ret=$?
    echo "### END($ret): `date`"

    return $ret
}

# make permissions predictable
umask 0022

_keep_env=0
_verbose=1
_no_log=0
_sandbox=
_keep_sandbox=0
_args=`getopt -o nkqvE -- "$@"`
if [ $? != 0 ] ; then echo "Args parsing failed..." >&2 ; exit 1 ; fi
eval set -- "$_args"

_args=( )
while true ; do
    case "$1" in
        -n) _no_log=1 ;;
        -k) _keep_sandbox=1 ;;
        -q) : $(( _verbose-- )) ;;
        -v) : $(( _verbose++ )) ;;
        -E) _keep_env=1 ;;
        --) shift ; break ;;
        *) echo "Internal error!" ; exit 1 ;;
    esac
    _args+=("$1")
    shift
done

if [[ $# -gt 1 ]] ; then
    echo "Unexpected arguments!" >&2
    exit 1
fi

trap on_exit EXIT

case "${1:-run}" in
    run)
        if [[ $_keep_env = 1 ]] ; then
            exec "$0" "${_args[@]}" __run
        else
            exec /usr/bin/env -i ${HOME+HOME="$HOME"} ${JENKINS_HOME+JENKINS_HOME="$JENKINS_HOME"} ${SHELL+SHELL="$SHELL"} ${SSH_AGENT_PID+SSH_AGENT_PID="$SSH_AGENT_PID"} ${SSH_AUTH_SOCK+SSH_AUTH_SOCK="$SSH_AUTH_SOCK"} ${TERM+TERM="$TERM"} ${USER+USER="$USER"} ${WORKSPACE+WORKSPACE="$WORKSPACE"} "$0" "${_args[@]}" __run
        fi
        ;;
    __run)
        cd "${0%/*}/workspace"
        if [[ $_no_log = 0 ]] ; then
            case "$_verbose" in
                0)
                    run_script >> ../log.txt 2>&1
                    ;;
                1)
                    set -o pipefail
                    {
                        {
                            run_script | tee -a ../log.txt
                        } 3>&1 1>&2- 2>&3- | tee -a ../log.txt
                    } 1>&2- 2>/dev/null
                    ;;
                *)
                    set -o pipefail
                    {
                        {
                            run_script | tee -a ../log.txt
                        } 3>&1 1>&2- 2>&3- | tee -a ../log.txt
                    } 3>&1 1>&2- 2>&3-
                    ;;
            esac
        else
            case "$_verbose" in
                0)
                    run_script 2>&1 > /dev/null
                    ;;
                1)
                    run_script > /dev/null
                    ;;
                *)
                    run_script
                    ;;
            esac
        fi
        ;;
    shell)
        if [[ $_keep_env = 1 ]] ; then
            exec /usr/bin/env BOB_CWD=/home/ben/app/mib3/dev/src/Websocket-server/tsd-audioclient-app-mib3/1/workspace LD_LIBRARY_PATH='' PATH=/opt/Java/jdk1.8.0_141/bin:/opt/Java/jdk1.8.0_141/jre/bin:/home/ben/app/BobBuildTool:/Library/Frameworks/Python.framework/Versions/3.5/bin:/Library/Frameworks/Python.framework/Versions/3.5/bin:/home/ben/bin:/opt/Java/jdk1.8.0_141/bin:/opt/Java/jdk1.8.0_141/jre/bin:/home/ben/app/BobBuildTool:/Library/Frameworks/Python.framework/Versions/3.5/bin:/Library/Frameworks/Python.framework/Versions/3.5/bin:/home/ben/bin:/opt/Java/jdk1.8.0_141/bin:/opt/Java/jdk1.8.0_141/jre/bin:/home/ben/app/BobBuildTool:/Library/Frameworks/Python.framework/Versions/3.5/bin:/Library/Frameworks/Python.framework/Versions/3.5/bin:/home/ben/bin:/opt/Java/jdk1.8.0_141/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/home/ben/sda4/Android/Sdk/platform-tools/adb:/home/ben/sda4/Android/Sdk/platform-tools/adb:/home/ben/sda4/Android/Sdk/platform-tools/adb "$0" "${_args[@]}" __shell
        else
            exec /usr/bin/env -i ${HOME+HOME="$HOME"} ${JENKINS_HOME+JENKINS_HOME="$JENKINS_HOME"} ${SHELL+SHELL="$SHELL"} ${SSH_AGENT_PID+SSH_AGENT_PID="$SSH_AGENT_PID"} ${SSH_AUTH_SOCK+SSH_AUTH_SOCK="$SSH_AUTH_SOCK"} ${TERM+TERM="$TERM"} ${USER+USER="$USER"} ${WORKSPACE+WORKSPACE="$WORKSPACE"} BOB_CWD=/home/ben/app/mib3/dev/src/Websocket-server/tsd-audioclient-app-mib3/1/workspace LD_LIBRARY_PATH='' PATH=/opt/Java/jdk1.8.0_141/bin:/opt/Java/jdk1.8.0_141/jre/bin:/home/ben/app/BobBuildTool:/Library/Frameworks/Python.framework/Versions/3.5/bin:/Library/Frameworks/Python.framework/Versions/3.5/bin:/home/ben/bin:/opt/Java/jdk1.8.0_141/bin:/opt/Java/jdk1.8.0_141/jre/bin:/home/ben/app/BobBuildTool:/Library/Frameworks/Python.framework/Versions/3.5/bin:/Library/Frameworks/Python.framework/Versions/3.5/bin:/home/ben/bin:/opt/Java/jdk1.8.0_141/bin:/opt/Java/jdk1.8.0_141/jre/bin:/home/ben/app/BobBuildTool:/Library/Frameworks/Python.framework/Versions/3.5/bin:/Library/Frameworks/Python.framework/Versions/3.5/bin:/home/ben/bin:/opt/Java/jdk1.8.0_141/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/home/ben/sda4/Android/Sdk/platform-tools/adb:/home/ben/sda4/Android/Sdk/platform-tools/adb:/home/ben/sda4/Android/Sdk/platform-tools/adb "$0" "${_args[@]}" __shell
        fi
        ;;
    __shell)
        cd "${0%/*}/workspace"
        rm -f ../audit.json.gz
        if [[ $_keep_env = 1 ]] ; then
            run /bin/bash -s 
        else
            run /bin/bash --norc -s 
        fi
        ;;
    *)
        echo "Unknown command" ; exit 1 ;;
esac

