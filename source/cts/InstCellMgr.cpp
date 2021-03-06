/**
* cts application
*
* @category		transcript manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstCellMgr.h"

#include <corelib/corelibdef.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/INSCellOption.h>

#include "CTSModule.h"

InstCellMgr::InstCellMgr():
inst_cells_nums_( 0)
{

}

InstCellMgr::~InstCellMgr()
{
	release();
}

void InstCellMgr::release()
{
	cell_states_.clear();

	for( INSTCELLMAPGROUP_MAP::iterator iter =mapgroup_states_.begin(); iter != mapgroup_states_.end(); ++iter)
		delete iter->second;
	mapgroup_states_.clear();
}

void InstCellMgr::initialize( SvrTeamOption* opt)
{
	inst_cells_nums_ =opt->get_inscellnums();
	inst_cells_.reset( FRAMEWK_NEW InstCellState[inst_cells_nums_]);

	SvrTeamOption::INSCELLOPTION_MAP::iterator eiter =opt->inscells_.end();
	int index =0;
	for( SvrTeamOption::INSCELLOPTION_MAP::iterator iter1 =opt->inscells_.begin(); iter1 != eiter; ++iter1, ++index)
	{
		INSCellOption* copt =iter1->second;
		InstCellState* pcstate =&(inst_cells_[index]);
		pcstate->set_option( copt);

		CSSLink* plink =CTSMODULE->get_csslink( copt->get_cssserverid());
		ACE_ASSERT( plink != 0);
		pcstate->set_csslink( plink);

		cell_states_[ pcstate->get_cellid()] =pcstate;

		//构建map分类
		InstCellMapGroup* mgroup =get_instcellsbymapid( pcstate->get_mapid());
		if( mgroup == 0)
		{
			mgroup =FRAMEWK_NEW InstCellMapGroup( pcstate->get_mapid());
			mapgroup_states_[ pcstate->get_mapid()] =mgroup;
		}

		mgroup->regist_instcell( pcstate);
	}
}

InstCellMapGroup* InstCellMgr::get_instcellsbymapid( int mapid)
{
	INSTCELLMAPGROUP_MAP::iterator fiter =mapgroup_states_.find( mapid);
	if( fiter == mapgroup_states_.end())
		return 0;
	return fiter->second;
}

InstCellState* InstCellMgr::get_canenterinstcellbymapid( int mapid)
{
	InstCellMapGroup* pgroup =get_instcellsbymapid( mapid);
	if( pgroup == 0)
		return 0;

	return pgroup->get_canentercell();
}

InstCellState* InstCellMgr::get_instcellbycellid( int cellid)
{
	INSTCELLSTATE_MAP::iterator fiter =cell_states_.find( cellid);
	if( fiter == cell_states_.end())
		return 0;
	return fiter->second;
}
