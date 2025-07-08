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

export LIB_DIR=$(pwd)/build_linux/lib # ehh.. this should work, but only in root dir for now
export DST_DIR=$INSTALL_PATH

echo Installing loader to "$DST_DIR/bin64/lib64"
#echo cp $LIB_DIR/fyp_* "$DST_DIR/bin64"
#echo cp $LIB_DIR/fyp_* "$DST_DIR/data"

#echo installing curl shim loader
ln -s "$LIB_DIR/fyp_loader.so" "$DST_DIR/bin64/lib64/fyp_loader.so"
ln -s "$LIB_DIR/fyp_shim_curl.so" "$DST_DIR/bin64/lib64/libcurl-gnutls.so.4"

echo Installing fyp modules to "$DST_DIR/bin64"
ln -s "$LIB_DIR/fyp_main.so" "$DST_DIR/bin64/fyp_main.so"
ln -s "$LIB_DIR/fyp_luajit.so" "$DST_DIR/bin64/fyp_luajit.so"
ln -s "$LIB_DIR/fyp_maphack.so" "$DST_DIR/bin64/fyp_maphack.so"

echo Installing fyp modules to "$DST_DIR/data"
ln -s "$LIB_DIR/fyp_main.so" "$DST_DIR/data/fyp_main.so"
ln -s "$LIB_DIR/fyp_luajit.so" "$DST_DIR/data/fyp_luajit.so"
ln -s "$LIB_DIR/fyp_maphack.so" "$DST_DIR/data/fyp_maphack.so"
