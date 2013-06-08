# カレントディレクトリのソースコードを検索
AUX_SOURCE_DIRECTORY(. srcs)

# 実行ファイル(.app形式)を設定
ADD_EXECUTABLE(BihinKanri MACOSX_BUNDLE ${srcs})

# FLTKに必要なライブラリを追加
TARGET_LINK_LIBRARIES(BihinKanri ${FLTK_LIBRARIES})
TARGET_LINK_LIBRARIES(BihinKanri ${FLTK_LIBRARIES})