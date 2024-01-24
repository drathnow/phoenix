#! /bin/sh
#  this is a quick and dirty transfer of passwords from old config file to new
#  config file...

set +x

##################  Done setup #################################

#
# function to find next line containing '"Password":' after the line containing the 
# specified key in file specified.
# Notes:
# It will echo out to caller the copy of the password line
# and return the line number of the file it found the password line on.
# on FAILURE: it returns line# 0 and blank line.
extractPassword ()
{
   local _ExpFile=$1
   local _ExpKey=$2
   # find the line containing unique key eg: Guest
   local findKey=`grep -ne $_ExpKey $_ExpFile`
   if [ ! "" = "$findKey" ] ; then
       # extract linenumber off front of the findkey line
       local findKeyNum=${findKey%%:*}
       #increment the key line number to be after find
       findKeyNum=$((findKeyNum+1))
       # look for 1st occurance of password in lines following (note we assign then echo to preserve white space...
        local tmpout1=`tail -n+${findKeyNum} $_ExpFile | grep -m1 -ne \"Password\":`
        local tmpout=`tail -n+${findKeyNum} $_ExpFile | grep -m1 -e \"Password\":`
       # local tmpout=${tmpout1##:*}
        local tmpout2=${tmpout1%%:*}
        echo "$tmpout"
        return $((tmpout2+findKeyNum-1))
   fi
  #by here we failed... return line number 0 and blank 
   echo ""
   return 0
}




# this is the keys to search for:
GuestKey="\"Guest\":.*{"
FieldKey="\"Field\":.*{"
AdminKey="\"Admin\":.*{"

#check dir and setup files
if [ $# -lt 2 ]; then
   echo "usage: $0 <path to old config.json> <path to new config.json>"
   exit 1
fi

if [ ! -d $1 ]; then
  echo "ERROR: can't find specified original config.json directory"
  exit 2
fi

if [ ! -d $2 ]; then
  echo "ERROR: can't find specified new config.json directory"
  exit 3
fi


FileIN=$1/config.json
FileOUT=$2/config.json

if [ ! -f $FileIN ]; then
  echo "ERROR: can't find config.json file in old directory"
  exit 4
fi
  
if [ ! -f $FileOUT ]; then
  echo "ERROR: can't find config.json file in new directory"
  exit 5
fi


# 1st find old passwords (blank if not found)
OldGuestPswd=$(extractPassword $FileIN $GuestKey)
OldFieldPswd=$(extractPassword $FileIN $FieldKey)
OldAdminPswd=$(extractPassword $FileIN $AdminKey)

# find new password line numbers (0 if not found)
tmpline=$(extractPassword $FileOUT $GuestKey)
NewGuestPswdNum=$?
tmpline=$(extractPassword $FileOUT $FieldKey)
NewFieldPswdNum=$?
tmpline=$(extractPassword $FileOUT $AdminKey)
NewAdminPswdNum=$?


#echo "old guest password: ***$OldGuestPswd***"
#echo "old field password :$OldFieldPswd"
#echo "old aDMIN password :$OldAdminPswd"

#echo "new guest password: $NewGuestPswd"
#echo "new field password :$NewFieldPswd"
#echo "new admin password :$NewAdminPswd"


#start replacing, if EVERYTHING fine...
if [ ! "" = "$OldGuestPswd" -a $((NewGuestPswdNum)) -gt 0 ]; then
# echo both guest found
 sed -i "${NewGuestPswdNum}s/.*/$OldGuestPswd/" $FileOUT
else 
 echo "ERROR: Did not restore Guest Password"
fi 

if [ ! "" = "$OldFieldPswd" -a $((NewFieldPswdNum)) -gt 0 ]; then
#  echo both field  found
  sed -i "${NewFieldPswdNum}s/.*/$OldFieldPswd/" $FileOUT
else 
 echo "ERROR: Did not restore Field Password"
fi 

if [ ! "" = "$OldAdminPswd" -a $((NewAdminPswdNum)) -gt 0 ]; then
#  echo both admin found
  sed -i "${NewAdminPswdNum}s/.*/$OldAdminPswd/" $FileOUT
else 
 echo "ERROR: Did not restore Admin Password"
fi 

echo "Password restoration complete...."


