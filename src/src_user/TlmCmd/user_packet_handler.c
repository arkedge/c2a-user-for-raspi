#pragma section REPRO
/**
 * @file
 * @brief  packet_handlerのコマンドルーター，コマンドアナライザのユーザー定義部分
 */
#include "user_packet_handler.h"
#include <src_core/TlmCmd/packet_list_util.h>


void PH_user_init(void)
{
}


PH_ACK PH_user_analyze_cmd(const CommonCmdPacket* packet)
{
  return PH_ACK_UNKNOWN;
}


CCP_EXEC_STS PH_user_cmd_router(const CommonCmdPacket* packet)
{
  return CCP_EXEC_ROUTING_FAILED;
}

#pragma section
