#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *      http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *      contact@openairinterface.org
# */
#---------------------------------------------------------------------

import os
import re
import sys

class spgwcConfigGen():
	def __init__(self):
		self.kind = ''
		self.s11c_name = ''
		self.sxc_name = ''
		self.apn = 'apn.oai.svc.cluster.local'
		self.dns1_ip = '192.168.18.129'
		self.dns2_ip = '8.8.4.4'
		self.fromDockerFile = False

	def GenerateSpgwcConfigurer(self):
		spgwcFile = open('./spgwc-cfg.sh', 'w')
		spgwcFile.write('#!/bin/bash\n')
		spgwcFile.write('\n')
		if self.fromDockerFile:
			spgwcFile.write('cd /openair-spgwc\n')
		else:
			spgwcFile.write('cd /home\n')
		spgwcFile.write('\n')
		spgwcFile.write('ifconfig lo:s5c 127.0.0.15 up\n')
		spgwcFile.write('echo "ifconfig lo:s5c 127.0.0.15 up --> OK"\n')
		spgwcFile.write('ifconfig lo:p5c 127.0.0.16 up\n')
		spgwcFile.write('echo "ifconfig lo:p5c 127.0.0.16 up --> OK"\n')
		spgwcFile.write('\n')
		spgwcFile.write('INSTANCE=1\n')
		if self.fromDockerFile:
			spgwcFile.write('PREFIX=\'/openair-spgwc/etc\'\n')
		else:
			spgwcFile.write('PREFIX=\'/usr/local/etc/oai\'\n')
		spgwcFile.write('\n')
		spgwcFile.write('MY_APN=\'' + self.apn + '\'\n')
		spgwcFile.write('MY_PRIMARY_DNS=\'' + self.dns1_ip + '\'\n')
		spgwcFile.write('MY_SECONDARY_DNS=\'' + self.dns2_ip + '\'\n')
		spgwcFile.write('\n')
		if not self.fromDockerFile:
			spgwcFile.write('mkdir -p $PREFIX\n')
			spgwcFile.write('cp etc/spgw_c.conf  $PREFIX\n')
			spgwcFile.write('\n')
		spgwcFile.write('declare -A SPGWC_CONF\n')
		spgwcFile.write('\n')
		spgwcFile.write('SPGWC_CONF[@INSTANCE@]=$INSTANCE\n')
		spgwcFile.write('SPGWC_CONF[@PID_DIRECTORY@]=\'/var/run\'\n')
		spgwcFile.write('SPGWC_CONF[@SGW_INTERFACE_NAME_FOR_S11@]=\'' + self.s11c_name + '\'\n')
		spgwcFile.write('SPGWC_CONF[@SGW_INTERFACE_NAME_FOR_S5_S8_CP@]=\'lo:s5c\'\n')
		spgwcFile.write('SPGWC_CONF[@PGW_INTERFACE_NAME_FOR_S5_S8_CP@]=\'lo:p5c\'\n')
		spgwcFile.write('SPGWC_CONF[@PGW_INTERFACE_NAME_FOR_SX@]=\'' + self.sxc_name + '\'\n')
		spgwcFile.write('SPGWC_CONF[@DEFAULT_DNS_IPV4_ADDRESS@]=$MY_PRIMARY_DNS\n')
		spgwcFile.write('SPGWC_CONF[@DEFAULT_DNS_SEC_IPV4_ADDRESS@]=$MY_SECONDARY_DNS\n')
		spgwcFile.write('SPGWC_CONF[@DEFAULT_APN@]=$MY_APN\n')
		spgwcFile.write('\n')
		spgwcFile.write('for K in "${!SPGWC_CONF[@]}"; do \n')
		spgwcFile.write('  egrep -lRZ "$K" $PREFIX | xargs -0 -l sed -i -e "s|$K|${SPGWC_CONF[$K]}|g"\n')
		spgwcFile.write('done\n')
		spgwcFile.write('\n')
		spgwcFile.write('exit 0\n')
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

argvs = sys.argv
argc = len(argvs)
cwd = os.getcwd()

mySpgwcCfg =  spgwcConfigGen()

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
		mySpgwcCfg.apn = matchReg.group(1)
	elif re.match('^\-\-dns1_ip=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-dns1_ip=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.dns1_ip = matchReg.group(1)
	elif re.match('^\-\-dns2_ip=(.+)$', myArgv, re.IGNORECASE):
		matchReg = re.match('^\-\-dns2_ip=(.+)$', myArgv, re.IGNORECASE)
		mySpgwcCfg.dns2_ip = matchReg.group(1)
	elif re.match('^\-\-from_docker_file', myArgv, re.IGNORECASE):
		mySpgwcCfg.fromDockerFile = True
	else:
		Usage()
		sys.exit('Invalid Parameter: ' + myArgv)

if mySpgwcCfg.kind == '':
	Usage()
	sys.exit('missing kind parameter')

if mySpgwcCfg.kind == 'SPGW-C':
	if mySpgwcCfg.s11c_name == '':
		Usage()
		sys.exit('missing S11 Interface Name on SPGW-C container')
	elif mySpgwcCfg.sxc_name == '':
		Usage()
		sys.exit('missing SX Interface Name on SPGW-C container')
	else:
		mySpgwcCfg.GenerateSpgwcConfigurer()
		sys.exit(0)
else:
	Usage()
	sys.exit('invalid kind parameter')
