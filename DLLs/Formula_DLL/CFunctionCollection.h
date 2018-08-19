#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:  Providing a list of build-in higher-level OpenPPL-symbols
//   and user-defined functions and lists. 
//
//   * CFunctionCollection (Formula.DLL) provides the actual functionality
//   * CSymbolEngineFunctionCollection (Symbols.DLL) provides the integration into the symbol-engine
//
//******************************************************************************

#include <map>
#include "LibDef.h"
#include "..\Formula_DLL\CDebugTab.h"
#include "..\..\Shared\CCritSec\CCritSec.h"

class COHScriptObject;

// We make the collection a symbol-engine because of 
// * reset-functions 
// * Evaluate()-function,
// * SymbolsProvided() for syntax-highlighting
class FORMULA_DLL_API CFunctionCollection {
  friend class CAutoplayerLogic;
  friend class CBetsizeInputBox;
  friend class CCasinoInterface;
  friend class CFormulaParser;
 public:
  CFunctionCollection();
  virtual ~CFunctionCollection();
 public:
  bool EvaluateSymbol(const CString name, double *result, bool log = false);
  double Evaluate(CString function_name, bool log = false);
  double EvaluateAutoplayerFunction(int function_code);
  bool EvaluatesToBinaryNumber(CString function_name);
 public: 
  CString SymbolsProvided();
 public:
  void UpdateOnHeartbeat();
 public:
  // To be called by
  //   * UpdateOnHeartbeat() 
  //   * the formula editor (f$debug and f$test)
  void ClearCache();
 public:
  // Deletes all user-defined functions
  // Keeps OpenPPL-library if correctly parsed.
   void DeleteAll(bool delete_read_only_library_functions, bool delete_user_defined);
  void SetEmptyDefaultBot();
 public:
  void Add(COHScriptObject *new_function);
  COHScriptObject *LookUp(CString name);
  bool Rename(CString from_name, CString to_name);
  void Delete(CString name);
  void SetFunctionText(CString name, CString content, bool read_only_library_symbol = false);
 public:
  bool Exists(CString name);
  // Generates smart error-messages on failure
  // To be used by the parser
  void VerifyExistence(CString name);
 public:
  void Save(CArchive &ar);
  void SaveObject(CArchive &ar, COHScriptObject *function_or_list);
  // For debugging
  void Dump();
 public:
  bool ParseAll();
  void SetOpenPPLLibraryLoaded(bool is_good) { _openPPL_library_loaded = is_good; }
  void CheckForDefaultFormulaEntries();
 public:
  bool BotLogicCorrectlyParsed();
  bool OpenPPLLibraryLoaded()            { return _openPPL_library_loaded; }
  bool IsOpenPPLProfile();
  bool IsOHScriptProfile();
 public:
  // Simply call GetFirst() first, then GetNext()
  // Once you receive NULL the end has been reached
  COHScriptObject *GetFirst();
  COHScriptObject *GetNext();
 public:
  CString FormulaName()     { return _formula_name; }
  CString FormulaPath()     { return _path; }
 public:
  CDebugTab* DebugTab() { return &_debug_tab;  }
 protected:
  void SetFormulaName(CString formula_name)	{ _formula_name = formula_name; }
  void SetPath(CString path)	  { _path = path; }
  // For OpenPPL, which evaluates f$preflop, ...
  // instead of f$beep, f$alli, ...
 private:
  void CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString &function_name);
  CString GetSimilarNameWithDifferentCases(CString function_name);
  bool CheckForOutdatedFunction(CString name);
  bool CheckForMisspelledOpenPPLMainFunction(CString name);
  void RemoveFromBinaryTree(CString function_name);
 private:
  void ExecuteSelftest();  
 private:
  std::map<CString, COHScriptObject*> _function_map;
  CString _formula_name;
  CString _path;
  bool _openPPL_library_loaded;
 private:
  CDebugTab _debug_tab;
 private:
  std::map<CString, COHScriptObject*>::iterator enumerator_it;
  CCritSec m_critsec;
};