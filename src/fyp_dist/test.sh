while getopts p: opts; do
   case ${opts} in
      p) INSTALL_PATH=${OPTARG} ;;
   esac
done

if [ "$INSTALL_PATH" = "" ]; then
    echo "-p specifies install absolute path"
    exit
fi

echo "Installing to $INSTALL_PATH"
