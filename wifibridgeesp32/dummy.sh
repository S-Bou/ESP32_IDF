set -e

while getopts x,e,a flag
do
    case "${flag}" in
        x) export="export";;
        e) erease="erease";;
        a) all="all";;
    esac
done

if [ "$export" = "export" ]
then
	. ../CecoMatrix/sources/esp-idf/export.sh
fi

if [ "$erease" = "erease" ]
then
	idf.py erase_flash
fi

if [ "$all" = "all" ]
then
	if [ "$(ls -A build)" ]
	then
		idf.py erase_flash
		idf.py fullclean
	else
		echo "Nothing to clean"
	fi
	# idf.py -G 'Unix Makefiles' build
		
else

echo -n "**** Do you want to clean cecodummyESP32?   [y/n]   "
read clean
echo -n "**** Do you want to build cecodummyESP32?   [y/n]   "
read build
echo -n "**** Do you want to flash cecodummyESP32?   [y/n]   "
read flash
echo -n "**** Do you want to monitor cecodummyESP32? [y/n]   "
read monitor

if [ "$clean" = "y" ] && [ -d build ]
then
	echo "Clean fw"
	idf.py clean
fi

if [ "$build" = "y" ]
then
	idf.py -G 'Unix Makefiles' build
fi

if [ "$flash" = "y" ]
then
	idf.py flash
fi

if [ "$monitor" = "y" ]
then
	idf.py monitor
fi

fi