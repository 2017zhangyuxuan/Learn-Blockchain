AUX_SOURCE_DIRECTORY(.  BLOCKCHAIN_SRC)
AUX_SOURCE_DIRECTORY(./block  BLOCK_SRC)
AUX_SOURCE_DIRECTORY(./transaction  TRANSACTION_SRC)
AUX_SOURCE_DIRECTORY(./wallet  WALLET_SRC)
add_library(BLOCKCHAIN_LIBRARY ${BLOCKCHAIN_SRC} ${BLOCK_SRC} ${TRANSACTION_SRC} ${WALLET_SRC})