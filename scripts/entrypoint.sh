#!/bin/bash

set -uo pipefail

CONFIG_DIR="/openair-spgwc/etc"
PUSH_PROTOCOL_OPTION=${PUSH_PROTOCOL_OPTION:-no}
TRIGGER_ASSOCIATION=${TRIGGER_ASSOCIATION:-false}
UE_MTU_IPV4=${UE_MTU_IPV4:-1464}


CUSTOM_UPF_FQDN=${CUSTOM_UPF_FQDN:-false}
UPF_FQDN=${UPF_FQDN:-upf-5g.kaloom.io}
if [[ $CUSTOM_UPF_FQDN == true ]]; then
        sed -i "s/gw@GW_ID@.spgw.node.epc.mnc@MNC03@.mcc@MCC@.@REALM@/$UPF_FQDN/g" ${CONFIG_DIR}/*.json
fi

for c in ${CONFIG_DIR}/*.json; do
    # grep variable names (format: ${VAR}) from template to be rendered
    VARS=$(grep -oP '@[a-zA-Z0-9_]+@' ${c} | sort | uniq | xargs)

    # create sed expressions for substituting each occurrence of ${VAR}
    # with the value of the environment variable "VAR"
    EXPRESSIONS=""
    for v in ${VARS}; do
        NEW_VAR=`echo $v | sed -e "s#@##g"`
        if [[ "${!NEW_VAR}x" == "x" ]]; then
            echo "Error: Environment variable '${NEW_VAR}' is not set." \
                "Config file '$(basename $c)' requires all of $VARS."
            exit 1
        fi
        EXPRESSIONS="${EXPRESSIONS};s|${v}|${!NEW_VAR}|g"
    done
    EXPRESSIONS="${EXPRESSIONS#';'}"

    # render template and inline replace config file
    sed -i "${EXPRESSIONS}" ${c}
done

exec "$@"
