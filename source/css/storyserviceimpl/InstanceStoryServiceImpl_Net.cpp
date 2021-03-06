/**
* css application
*
* @category		Transcript Story Service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstanceStoryServiceImpl.h"

#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>
#include <reslib/deploy/CSSOption.h>
#include <reslib/world/WorldInfoConfig.h>
#include <reslib/world/ToTranscriptConfig.h>

#include "../CSSModule.h"

USE_PROTOCOL_NAMESPACE;

void InstanceStoryServiceImpl::gts_instenter_req( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}

void InstanceStoryServiceImpl::cts_instenter_req( BasicProtocol* p, bool& autorelease)
{
	Pro_AppEnterIns_req* req =dynamic_cast<Pro_AppEnterIns_req*>(p);

	if( svr_state_ != TRANSCRIPT_SVRST_CANENTER)
	{
		//重置副本缓存
		CSSMODULE->reset_adapterplayer_instcache( req->get_uuidglobalindex());

		Pro_AppEnterIns_ack* ack =PROTOCOL_NEW Pro_AppEnterIns_ack();
		PRO_UUID_FILL2( ack, req);
		ack->result_ =7;	//不允许进入副本
		CSSMODULE->send_to_cts( ack);
		return;
	}

	//注销玩家
	Player* pold =get_player( p->uuid_);
	if( pold != 0)
		this->player_unregist( pold);

	//注册玩家
	Player* user =this->player_regist( p->uuid_, req->chrid_);
	ACE_ASSERT( user != 0);

	//获取副本出生点信息
	ToTranscriptConfig& tconf =WORLDINFO->get_totranscriptconfig();
	transcript_teleport_info* pteleport =tconf.get_teleportinfobyid( req->telid_);
	user->set_position( pteleport->instmap_pos_);

	Pro_AppEnterIns_ack* ack =PROTOCOL_NEW Pro_AppEnterIns_ack();
	PRO_UUID_FILL2( ack, req);
	ack->result_ =0;
	ack->cssindex_ =CSSMODULE->get_owneropt()->server_index_;
	CSSMODULE->send_to_cts( ack);
}

void InstanceStoryServiceImpl::cts_instenter_ack( BasicProtocol* p, bool& autorelease)
{
	ACE_ASSERT( FALSE && "shouldn't receive this protocol");
}

void InstanceStoryServiceImpl::cts_enterinstovertime_ntf( BasicProtocol* p, bool& autorelease)
{
	Pro_AppEnterInsOvertime_ntf* ntf =dynamic_cast<Pro_AppEnterInsOvertime_ntf*>( p);
	CSSMODULE->reset_adapterplayer_instcache( ntf->get_uuidglobalindex());

	CSS_GETPLAYER_FROMSTORY( user, p);

	if( user->is_initfinish())
	{
		//发送退出协议
		Pro_Logout_ntf* ntf =PROTOCOL_NEW Pro_Logout_ntf();
		ntf->chrid_ =user->get_id();
		scene_manage_->broadcast_inview( user, ntf, false);

		scene_manage_->unitobj_leavemap( user);
	}

	this->player_unregist( p->uuid_);
}

void InstanceStoryServiceImpl::gts_instenterconfirm_ntf( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	Pro_AppEnterInsConfirm_ntf* ntf =dynamic_cast<Pro_AppEnterInsConfirm_ntf*>( p);
	CSSMODULE->send_to_cts( ntf);
	autorelease =false;

	//发送获取玩家信息请求
	Pro_ChrLoad_req* lntf =PROTOCOL_NEW Pro_ChrLoad_req();
	PRO_UUID_FILL( lntf, user->global_index_, user->uuid_);
	lntf->chrid_ =user->chrid_;
	lntf->cssindex_ =CSSMODULE->get_owneropt()->server_index_;

	CSSMODULE->send_to_dpx( lntf);
}

void InstanceStoryServiceImpl::gts_instquit_req( BasicProtocol* p, bool& autorelease)
{
	CSS_GETPLAYER_FROMSTORY( user, p);

	CSSMODULE->reset_adapterplayer_instcache( p->get_uuidglobalindex());

	if( user->is_initfinish())
	{
		//发送退出协议
		Pro_Logout_ntf* ntf =PROTOCOL_NEW Pro_Logout_ntf();
		ntf->chrid_ =user->get_id();
		scene_manage_->broadcast_inview( user, ntf, false);

		scene_manage_->unitobj_leavemap( user);
	}

	this->player_unregist( p->uuid_);

	CSSMODULE->send_to_cts( p);
	autorelease =false;
}
