/**
* reslib application
*
* @category		服务器用到的公共状态定义
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __SERVICE_DEFINE__H__
#define __SERVICE_DEFINE__H__

//副本服务器状态
enum TRANSCRIPT_SERVER_STATE
{
	//未注册
	TRANSCRIPT_SVRST_UNREGIST =0,
	//不可用
	TRANSCRIPT_SVRST_DISABLE,
	//允许进入
	TRANSCRIPT_SVRST_CANENTER,
	//拒绝进入
	TRANSCRIPT_SVRST_REJECTENTER,
};

#endif	//__SERVICE_DEFINE__H__
