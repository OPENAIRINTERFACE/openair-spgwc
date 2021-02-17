#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *	  http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *	  contact@openairinterface.org
# */
#---------------------------------------------------------------------

import os
import re
import sys
import ipaddress

class spgwcConfigGen():
	def __init__(self):
		self.kind = ''
		self.s11c_name = ''
		self.sxc_name = ''
		self.apn1 = 'apn1.oai.svc.cluster.local'
		self.apn2 = 'apn2.oai.svc.cluster.local'
		self.dns1_ip = ipaddress.ip_address('192.168.18.129')
		self.dns2_ip = ipaddress.ip_address('8.8.4.4')
		self.uePoolRange = ipaddress.ip_network('12.1.1.0/24')
		self.ue_pool1_range = '12.1.1.2 - 12.1.1.254'
		self.ue_pool2_range = '192.168.21.2 - 192.168.21.254'
		self.fromDockerFile = False
		self.envForEntrypoint = False
		self.pushProtocolOption = 'false'

	def GenerateSpgwcConfigurer(self):
		spgwcFile = open('./spgwc-cfg.sh', 'w')
		spgwcFile.write('#!/bin/bash\n')
		spgwcFile.write('\n')
		if self.fromDockerFile:
			spgwcFile.write('cd /openair-spgwc\n')
		else:
			spgwcFile.write('cd /home\n')
		spgwcFile.write('\n')
		if self.fromDockerFile:
			spgwcFile.write('PREFIX=\'/openair-spgwc/etc\'\n')
		else:
			spgwcFile.write('PREFIX=\'/usr/local/etc/oai\'\n')
		spgwcFile.write('\n')
		spgwcFile.write('MY_APN=\'' + self.apn1 + '\'\n')
		spgwcFile.write('MY_PRIMARY_DNS=\'' + str(self.dns1_ip) + '\'\n')
		spgwcFile.write('MY_SECONDARY_DNS=\'' + str(self.dns2_ip) + '\'\n')
		spgwcFile.write('\n')
		if not self.fromDockerFile:
			spgwcFile.write('mkdir -p $PREFIX\n')
			spgwcFile.write('cp etc/spgw_c.conf  $PREFIX\n')
			spgwcFile.write('\n')
		spgwcFile.write('declare -A SPGWC_CONF\n')
		spgwcFile.write('\n')
		spgwcFile.write('SPGWC_CONF[@PID_DIRECTORY@]=\'/var/run\'\n')
		spgwcFile.write('SPGWC_CONF[@SGW_INTERFACE_NAME_FOR_S11@]=\'' + self.s11c_name + '\'\n')
		spgwcFile.write('SPGWC_CONF[@PGW_INTERFACE_NAME_FOR_SX@]=\'' + self.sxc_name + '\'\n')
		spgwcFile.write('SPGWC_CONF[@SGW_IP_FOR_S5_S8_CP@]=127.0.0.11/8\n')
		spgwcFile.write('SPGWC_CONF[@PGW_IP_FOR_S5_S8_CP@]=127.0.0.12/8\n')
		spgwcFile.write('SPGWC_CONF[@DEFAULT_DNS_IPV4_ADDRESS@]=$MY_PRIMARY_DNS\n')
		spgwcFile.write('SPGWC_CONF[@DEFAULT_DNS_SEC_IPV4_ADDRESS@]=$MY_SECONDARY_DNS\n')
		spgwcFile.write('SPGWC_CONF[@DEFAULT_APN@]=$MY_APN\n')
		spgwcFile.write('SPGWC_CONF[@UE_IP_ADDRESS_POOL@]=\'' + self.ue_pool1_range + '\'\n')
		spgwcFile.write('SPGWC_CONF[@PUSH_PROTOCOL_OPTION@]=\'' + self.pushProtocolOption + '\'\n')
		spgwcFile.write('\n')
		spgwcFile.write('for K in "${!SPGWC_CONF[@]}"; do \n')
		spgwcFile.write('  egrep -lRZ "$K" $PREFIX | xargs -0 -l sed -i -e "s|$K|${SPGWC_CONF[$K]}|g"\n')
		spgwcFile.write('done\n')
		spgwcFile.write('\n')
		spgwcFile.write('exit 0\n')
		spgwcFile.close()

	def GenerateSpgwcEnvList(self):
		spgwcFile = open('./spgwc-env.list', 'w')
		spgwcFile.write('# Environment Variables used by the OAI-SPGW-C Entrypoint Script\n')
		spgwcFile.write('SGW_INTERFACE_NAME_FOR_S11=' + self.s11c_name + '\n')
		spgwcFile.write('PGW_INTERFACE_NAME_FOR_SX=' + self.sxc_name + '\n')
		spgwcFile.write('DEFAULT_DNS_IPV4_ADDRESS=' + str(self.dns1_ip) + '\n')
		spgwcFile.write('DEFAULT_DNS_SEC_IPV4_ADDRESS=' + str(self.dns2_ip) + '\n')
		spgwcFile.write('PUSH_PROTOCOL_OPTION=' + self.pushProtocolOption + '\n')
		spgwcFile.write('APN_NI_1=' + self.apn1 + '\n')
		spgwcFile.write('APN_NI_2=' + self.apn2 + '\n')
		spgwcFile.write('DEFAULT_APN_NI_1=' + self.apn1 + '\n')
		spgwcFile.write('UE_IP_ADDRESS_POOL_1=' + self.ue_pool1_range + '\n')
		spgwcFile.write('UE_IP_ADDRESS_POOL_2=' + self.ue_pool2_range + '\n')
		spgwcFile.write('MCC=208\n')
		spgwcFile.write('MNC=99\n')
		spgwcFile.write('MNC03=099\n')
		spgwcFile.write('TAC=1\n')
		spgwcFile.write('GW_ID=1\n')
		spgwcFile.write('REALM=openairinterface.org\n')
		spgwcFile.close()

#-----------------------------------------------------------
# Usage()
#-----------------------------------------------------------
def Usage():
	print('----------------------------------------------------------------------------------------------------------------------')
	print('generateConfigFiles.py')
	print('   Prepare a bash script to be run in the workspace where SPGW-C is being built.')
	print('   That bash script will copy configuration template files and adapt to your configuration.')
	print('----------------------------------------------------------------------------------------------------------------------')
	print('Usage: python3 generateConfigFiles.py [options]')
	print('  --help  Show this help.')
	print('------------------------------------------------------------------------------------------------- SPGW-C Options -----')
	print('  --kind=SPGW-C')
	print('  --s11c=[SPGW-C S11 Interface Name]')
	print('  --sxc=[SPGW-C SX Interface Name]')
	print('  --from_docker_file')
	print('------------------------------------------------------------------------------------------- SPGW-C Not Mandatory -----')
	print('  --apn=[Access Point Name]')
	print('  --dns1_ip=[First DNS IP address]')
	print('  --dns2_ip=[Second DNS IP address]')
	print('  --network_ue_ip=[UE IP pool range in CICDR format, for example 12.1.1.0/24. The attached UE will be allocated an IP address in that range.]')
	print('  --push_protocol_option=[yes or no, no is default]')
	print('  --env_for_entrypoint    [generates a spgwc-env.list interpreted by the entrypoint]')

argvs = sys.argv
argc = len(argvs)
cwd = os.getcwd()

mySpgwcCfg = spgwcConfigGen()
uePoolRangedChanged = False

while len(argvs) > 1:
	myArgv = argvs.pop(1)
	if re.match('^\-\-help$', myArgv, re.IGNORECASE):
		Usage()
		sys.exit(0)
	elif re.match('^\-\-kind=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-kind=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.kind = matchReg.group(1)
	elif re.match('^\-\-s11c=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-s11c=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.s11c_name = matchReg.group(1)
	elif re.match('^\-\-sxc=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-sxc=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.sxc_name = matchReg.group(1)
	elif re.match('^\-\-apn=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-apn=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.apn1 = matchReg.group(1)
	elif re.match('^\-\-dns1_ip=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-dns1_ip=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.dns1_ip = ipaddress.ip_address(matchReg.group(1))
	elif re.match('^\-\-dns2_ip=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-dns2_ip=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.dns2_ip = ipaddress.ip_address(matchReg.group(1))
	elif re.match('^\-\-from_docker_file', myArgv, re.IGNORECASE):
		mySpgwcCfg.fromDockerFile = True
	elif re.match('^\-\-env_for_entrypoint', myArgv, re.IGNORECASE):
		mySpgwcCfg.envForEntrypoint = True
	elif re.match('^\-\-network_ue_ip', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-network_ue_ip=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.uePoolRange = ipaddress.ip_network(matchReg.group(1))
		uePoolRangedChanged = True
	elif re.match('^\-\-push_protocol_option', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-push_protocol_option=(.+)$', myArgv, re.IGNORECASE)
		pushProtOption = matchReg.group(1)
		if pushProtOption == 'yes' or pushProtOption == 'Yes' or pushProtOption == 'YES':
			mySpgwcCfg.pushProtocolOption = 'yes'
	else:
		Usage()
		sys.exit('Invalid Parameter: ' + myArgv)

if mySpgwcCfg.kind == '':
	Usage()
	sys.exit('missing kind parameter')

if mySpgwcCfg.kind == 'SPGW-C':
	if uePoolRangedChanged:
		initAddr = mySpgwcCfg.uePoolRange.network_address
		endAddr  = initAddr + mySpgwcCfg.uePoolRange.num_addresses - 1
		initAddr += 2
		mySpgwcCfg.ue_pool1_range = str(initAddr) + ' - ' + str(endAddr)
	if mySpgwcCfg.s11c_name == '':
		Usage()
		sys.exit('missing S11 Interface Name on SPGW-C container')
	elif mySpgwcCfg.sxc_name == '':
		Usage()
		sys.exit('missing SX Interface Name on SPGW-C container')
	else:
		if mySpgwcCfg.envForEntrypoint:
			mySpgwcCfg.GenerateSpgwcEnvList()
		else:
			mySpgwcCfg.GenerateSpgwcConfigurer()
		sys.exit(0)
else:
	Usage()
	sys.exit('invalid kind parameter')
