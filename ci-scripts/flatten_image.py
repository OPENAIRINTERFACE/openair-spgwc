"""
 Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 contributor license agreements.  See the NOTICE file distributed with
 this work for additional information regarding copyright ownership.
 The OpenAirInterface Software Alliance licenses this file to You under
 the OAI Public License, Version 1.1  (the "License"); you may not use this file
 except in compliance with the License.
 You may obtain a copy of the License at

   http://www.openairinterface.org/?page_id=698

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-------------------------------------------------------------------------------
 For more information about the OpenAirInterface (OAI) Software Alliance:
   contact@openairinterface.org
"""

import argparse
import re
import subprocess
import sys

def main() -> None:
    args = _parse_args()
    status = perform_flattening(args.tag)
    sys.exit(status)

def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description='Flattening Image')

    parser.add_argument(
        '--tag', '-t',
        action='store',
        required=True,
        help='Image Tag in image-name:image tag format',
    )
    return parser.parse_args()

def perform_flattening(tag):
    # First detect which docker/podman command to use
    cli = ''
    image_prefix = ''
    cmd = 'which podman || true'
    podman_check = subprocess.check_output(cmd, shell=True, universal_newlines=True)
    if re.search('podman', podman_check.strip()):
        cli = 'sudo podman'
        image_prefix = 'localhost/'
    if cli == '':
        cmd = 'which docker || true'
        docker_check = subprocess.check_output(cmd, shell=True, universal_newlines=True)
        if re.search('docker', docker_check.strip()):
            cli = 'docker'
            image_prefix = ''
    if cli == '':
        print ('No docker / podman installed: quitting')
        return -1
    print (f'Flattening {tag}')
    # Creating a container
    cmd = cli + ' run --name test-flatten --entrypoint /bin/true -d ' + tag
    print (cmd)
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    # Export / Import trick
    cmd = cli + ' export test-flatten | ' + cli + ' import '
    # Bizarro syntax issue with podman
    if cli == 'docker':
      cmd += ' --change "ENV PATH /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" '
    else:
      cmd += ' --change "ENV PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" '
    cmd += ' --change "WORKDIR /openair-spgwc" '
    cmd += ' --change "EXPOSE 2123/udp" '
    cmd += ' --change "EXPOSE 8805/udp" '
    cmd += ' --change "LABEL use-json-file=\\"true\\"" '
    cmd += ' --change "LABEL support-multi-sgwu-instances=\\"true\\"" '
    cmd += ' --change "CMD [\\"/openair-spgwc/bin/oai_spgwc\\", \\"-c\\", \\"/openair-spgwc/etc/spgw_c.json\\", \\"-o\\"]" '
    cmd += ' --change "ENTRYPOINT [\\"/bin/bash\\", \\"/openair-spgwc/bin/entrypoint.sh\\"]" '
    cmd += ' - ' + image_prefix + tag
    print (cmd)
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    # Remove container
    cmd = cli + ' rm -f test-flatten'
    print (cmd)
    subprocess.check_output(cmd, shell=True, universal_newlines=True)

    # At this point the original image is a dangling image.
    # CI pipeline will clean up (`image prune --force`)
    return 0

if __name__ == '__main__':
    main()
