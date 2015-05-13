// StringSet.H: interface for the CIVStringSet class.
//
// Written 30 June 2002 by Scot T Brennecke
// Thanks to Moishe Halibard and Moshe Rubin for their article,
//    "A Multiple Substring Search Algorithm" in the June 2002
//    edition of C/C++ Users Journal.  This class is based on
//    the algorthim therein described, but extended to return
//    all strings and use MFC classes.
//
// Modified 24 Apr 2010 by Scot T Brennecke

#pragma once

#pragma warning(disable: 4100 4786)
#include <set>
#include <vector>
#include <string>
#include <list>

#ifdef _UNICODE
typedef std::wstring _tstring ;
#else
typedef std::string _tstring ;
#endif

struct SEntry
{
    SEntry( unsigned long dwState = 0UL, unsigned long dwIndex = 0UL )
        : m_dwState( dwState ), m_dwIndex( dwIndex ) {}

    unsigned long m_dwState ;
    unsigned long m_dwIndex ;
} ;

class CIVStringSet : public std::vector<_tstring>
{
public:
    CIVStringSet( unsigned long dwInitialWidth = 64 ) ;  // Initial width of FSM
    virtual ~CIVStringSet() ;

    bool        Add( LPCTSTR pszWord ) ;                         // a single word
    bool        Add( const _tstring & rstrWord ) ;               // a single word
    size_t      Add( LPCTSTR pszWords, LPCTSTR pszDelims ) ;     // multiple words, delimited with chars from pszDelims
    size_t      Add( LPCTSTR pszzWords, size_t nWords ) ;        // nWords words, each 0 term'd, with extra 0 at end
    size_t      Add( const std::set<_tstring> & sstrWords ) ;    // all the elements of a set of strings
    size_t      Add( const std::vector<_tstring> & astrWords ) ; // all the elements of an array of strings
    size_t      Add( const std::list<_tstring> & lstrWords ) ;   // all the elements of a list of strings

    void        SetDelims( LPCTSTR pszDelims ) { m_strDelims = pszDelims ; } // delimiters to use in word search
    UINT        FindFirstIn( const _tstring & strText, size_t & rnFirst ) ;  // Begin iteration
    UINT        FindNext( size_t & rnNext ) ;                                // Continue iteration

    typedef std::pair<size_t,UINT>              CWordPosPair ;     // first is index of word in array, second is position in text
    typedef std::list< std::pair<size_t,UINT> > CWordPosPairList ; // list of pairs to be returned by FindAllIn
    size_t      FindAllIn( _tstring strText, CWordPosPairList & rlstrWords ) ; // Iterate all at once and make list
    
protected:
    std::vector< std::vector< SEntry > > m_apnFSM ;   // Finite State Machine.
    unsigned long m_dwMaxUsedState ;  // largest state value used
    _tstring      m_strSearch ;       // Current search string
    _tstring      m_strDelims ;       // Current delimiters for search
    UINT          m_nCurTextChar ;    // Current position in search string

    bool          InsertWord( LPCTSTR pszWord, unsigned long dwIndex ) ; // put the new word into the FSM for given index

private:
    // Hide several base class members that shouldn't be used
    // Using these members could cause the index numbers of strings to change, throwing
    //     off the embedded index entries in the FSM
    iterator insert(const_iterator _Where, const _tstring & _Val);
    void insert(const_iterator _Where, size_type _Count, const _tstring & _Val);
    template<class _Iter> void insert(const_iterator _Where, _Iter _First, _Iter _Last);
//    template<class _Iter> void _Insert(const_iterator _Where, _Iter _First, _Iter _Last, std::_Int_iterator_tag);
    template<class _Iter> void _Insert(const_iterator _Where, _Iter _First, _Iter _Last, std::input_iterator_tag);
    template<class _Iter> void _Insert(const_iterator _Where, _Iter _First, _Iter _Last, std::forward_iterator_tag);
//    void _Reverse(pointer _First, pointer _Last);
    iterator erase(const_iterator _Where);
    iterator erase(const_iterator _First_arg, const_iterator _Last_arg);
} ;
