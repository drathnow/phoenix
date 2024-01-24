#
# Template file for localdev.mk.  This file should be placed in a directory under the users
# home directory called ~/.zdev and rename to localdev.mk.  Variables defined in this file
# support the ZIOS build process
#

#
# Defines the IP address of the FG used by developers for development
#
REMOTE_FIG_HOST=<your-fgs-ip>

#
# Used by the 'copyarm' target.  Defines the host and root folder for zios. 
#
# NOTE: For this to work, developers must create and add their public ssh key in .ssh/authorized_keys
# under root user's home directory
#
REMOTE_ZIOS_ROOT=root@$(REMOTE_FIG_HOST):/zioshome

#
# Used by the 'package' target.  If set, the deploy file will be copied to this location
# at the end of the build.
#
PACKAGE_LOC=<your-package-location>

#
# Used by the 'deploy' target. This specifies where zios build script will place artifacts
# required to run
#
DEPLOY_ROOT=<your-deploy-location>