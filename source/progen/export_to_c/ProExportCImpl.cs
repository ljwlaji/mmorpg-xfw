﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

using progen.pro_meta;

namespace progen.export_to_c
{
    public partial class ProExportCImpl : IProExport
    {
        private string path_root_ = "";
        private GenerateStream cur_stream_ = null;

        //all include file that protocol cpp file will used
        private StringBuilder include_files_ = new StringBuilder();

        #region IProExport implementation

        public void GenerateProLib()
        {
            //make output directory
            path_root_ = ExportConfig.Instance().Export2CPath;
            FileUtil.DelDir(path_root_);
            Thread.Sleep(3000);
            FileUtil.MakeDir( ref path_root_);

            //generate macro define
            GenerateMacroDefine();

            GenerateEnumStruct();

            GenerateGlobalStruct();

            GenerateProtocols();
        }

        public void DeployProLib()
        {
            string targetpath =ExportConfig.Instance().Target2CPath;
            FileUtil.DelDir(targetpath);
            Thread.Sleep(3000);
            FileUtil.MakeDir(ref targetpath);

            //copy generate files into target dir
            List<string> allfile = FileUtil.ListFilesOfDir( path_root_, "*.*");
            foreach (string f in allfile)
            {
                string src = FileUtil.GetFile(path_root_, f);
                string des = FileUtil.GetFile(targetpath, f);
                File.Copy(src, des, true);
            }
        }

        #endregion

        #region export function

        protected void MakeOutputFile( string path, string file)
        {
            if (cur_stream_ != null)
                cur_stream_.FinishGenerate();

            cur_stream_ = new GenerateStream(path, file, ExportCConst.FileHeadInfo);
        }

        protected void GenHFileHeaderB( string filename)
        {
            filename =filename.ToUpper();
            cur_stream_.WriteString(String.Format("#ifndef __{0}__H__", filename), 0);
            cur_stream_.WriteString(String.Format("#define __{0}__H__", filename), 0);
        }

        protected void GenHFileHeaderE( string filename)
        {
            filename = filename.ToUpper();

            cur_stream_.WriteEmptyLine();
            cur_stream_.WriteString(String.Format("#endif //__{0}__H__", filename), 0);
        }

        protected string DataType2CType( ProMetaMember sm)
        {
            string type = "";
            if (sm.Type == ProMetaConst.DATATYPE_LOCALSTRUCT)
                type = sm.RefLocalStruct.Name;
            else if (sm.Type == ProMetaConst.DATATYPE_GLOBALSTRUCT)
                type = String.Format("PRO::{0}", sm.RefGlobalStruct.Name);
            else
                type = DataType2CType((StructMember)sm);

            if (sm.IsList)
                return String.Format("std::list< {0} >", type);
            else
                return type;
        }

        protected string DataType2CType(StructMember sm)
        {
            string type = "";
            if (sm.Type == ProMetaConst.DATATYPE_INT8)
                type ="S_INT_8";
            else if (sm.Type == ProMetaConst.DATATYPE_INT16)
                type = "S_INT_16";
            else if (sm.Type == ProMetaConst.DATATYPE_INT32)
                type = "S_INT_32";
            else if (sm.Type == ProMetaConst.DATATYPE_INT64)
                type = "S_INT_64";
            else if (sm.Type == ProMetaConst.DATATYPE_UINT32)
                type = "S_UINT_32";
            else if (sm.Type == ProMetaConst.DATATYPE_FLOAT32)
                type = "S_FLOAT_32";
            else if (sm.Type == ProMetaConst.DATATYPE_TIMESTAMP)
                type = "S_TIMESTAMP";
            else if (sm.Type == ProMetaConst.DATATYPE_STATICSTRING)
            {
                if (sm.MacroLen != null)
                    type = String.Format("StaticString< {0} >", sm.MacroLen.Name);
                else
                    type = String.Format("StaticString< {0} >", sm.ILen);
            }
            else if(sm.Type == ProMetaConst.DATATYPE_SERIALIZEOBJ)
                type = "SerializeObject*";
            else
                throw new pro_meta.exception.AnalysisException(String.Format("{0} member type not supported", sm.Type));

            if (sm.IsList && sm.Type != ProMetaConst.DATATYPE_SERIALIZEOBJ)
                return String.Format("std::list< {0} >", type);
            else
                return type;
        }

        #endregion
    }
}
