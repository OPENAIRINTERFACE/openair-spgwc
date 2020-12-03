# Build and deploying OAI-SPGW-C in OpenShift cluster #

## Pre-requisite ##

On a `RHEL8` physical machine (or a virtual machine) connected to the OpenShift Cluster, recover the entitlement and the RH subscription manager configs:

```bash
oc create configmap rhsm-conf --from-file /etc/rhsm/rhsm.conf
oc create configmap rhsm-ca --from-file /etc/rhsm/ca/redhat-uep.pem

oc create secret generic etc-pki-entitlement --from-file /etc/pki/entitlement/{NUMBER_ON_YOUR_COMPUTER}.pem --from-file /etc/pki/entitlement/{NUMBER_ON_YOUR_COMPUTER}-key.pem
```

These configmaps and secret will be shared by all the build configs in your OC project. No need to do it each time.

## Launching the Build ##

On a machine connected to the OpenShift Cluster, create the target image stream and the build configuration.

Note that the project name is currently hard-coded to `oai`.

```bash
git clone https://github.com/OPENAIRINTERFACE/openair-spgwc.git
cd openair-spgwc
git checkout develop
oc apply -f openshift/oai-spgwc-image-stream.yml
oc apply -f openshift/oai-spgwc-build-config.yml
```

Note that this step has to be done once before the first build and if you modify the yaml files.

Then anytime you want to build:

```bash
oc start-build oai-spgwc-build-config --follow
```

The `--follow` might break.

```bash
oc logs build/oai-spgwc-build-config-XYZ --follow
```

where `XYZ` is the build number.

You should see a successful buid when :

```bash
...
Pushing image image-registry.openshift-image-registry.svc:5000/oai/oai-spgwc:onap-0.1.0 ...
Getting image source signatures
Copying blob sha256:087e0da17c56af1aaf0a1eba37169b46f6c46936b554b7e13f11165dac4ba8ef
...
Copying blob sha256:0adcf0e0c86fbba84ed7a22a57a2cd4d7695c1ce858dd03f03dc3002880b8061
Copying config sha256:73330dfa1a733335355324848a02ba1899f9d125559979f9911f3dedcd4450ca
Writing manifest to image destination
Storing signatures
Successfully pushed image-registry.openshift-image-registry.svc:5000/oai/oai-spgwc@sha256:c1de087af5515d22e40486fb181f6ff65a7f34788d8174aa122d686667042cf9
Push successful
```
