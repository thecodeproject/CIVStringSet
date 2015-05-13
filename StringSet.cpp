// StringSet.Cpp: implementation of the CIVStringSet class.
//
// Written 12 June 2002 by Scot T Brennecke
// Thanks to Moishe Halibard and Moshe Rubin for their article,
//    "A Multiple Substring Search Algorithm" in the June 2002
//    edition of C/C++ Users Journal.  This class is based on
//    the algorthim therein described, but extended to return
//    all strings and use MFC classes.
//
// Modified 24 Apr 2010 by Scot T Brennecke


#include "StdAfx.H"
#include "StringSet.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__ ;
#define new DEBUG_NEW
#endif

// This will dictate one dimension of the vectors, and can dramatically affect memory use
// Using 0x7F will limit to typical ANSI characters
const size_t c_nMaxChar = 0x7F ;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                       C I V S t r i n g S e t
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CIVStringSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CIVStringSet::CIVStringSet( unsigned long dwInitialWidth /*= 64*/ )
: m_apnFSM( dwInitialWidth, std::vector<SEntry>( c_nMaxChar ) ),
    m_dwMaxUsedState( 0 ),
    m_strDelims( _T(" \t\n\r,.!#*(){}[]?/\\\"'") ),
    m_nCurTextChar( 0 )
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~CIVStringSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CIVStringSet::~CIVStringSet()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Add
//    several variations, the first being the most basic
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool CIVStringSet::Add( LPCTSTR pszWord )
{
    bool bRetVal = false ;

    // Insert the word into the FSM, then add it to the array
    if ( ( pszWord != NULL )
      && InsertWord( pszWord, static_cast<unsigned long>(size() + 1) )
       )
    {
        _tstring strWord(pszWord) ;
        push_back( strWord ) ;
        bRetVal = true ;
    }

    return bRetVal ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Add a string
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool CIVStringSet::Add( const _tstring & rstrWord )
{
    bool bRetVal = false ;

    // Insert the word into the FSM, then add it to the array
    if ( InsertWord( rstrWord.c_str(), static_cast<unsigned long>(size() + 1) ) )
    {
        push_back( rstrWord ) ;
        bRetVal = true ;
    }

    return bRetVal ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Add multiple words, delimited with chars from pszDelims
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t CIVStringSet::Add( LPCTSTR pszWords, LPCTSTR pszDelims )
{
    size_t nCount = 0 ;

    // Start with the entire string
    _tstring strUnparsed( pszWords ),
             strToken ;

    if ( pszDelims == NULL )
        pszDelims = m_strDelims.c_str() ;

    // stop iterating when we find no more words following delimiters
    while ( ! strUnparsed.empty() )
    {
        // Extract the first token
        size_t nTokSize = _tcscspn( strUnparsed.c_str(), pszDelims ) ;
        strToken = strUnparsed.substr( 0, nTokSize ) ;
        if ( ! strToken.empty() )
        {
            if ( Add( strToken ) )  // add it to the FSM and the array
                nCount++ ;
            else
                break ;
        }

        // Now, move beyond the token just extracted and the delimiters
        size_t nTokenLen = strToken.length() ;
        if ( nTokenLen < strUnparsed.length() )
        {
            size_t nDelimLen = _tcsspn( &strUnparsed[nTokenLen], pszDelims ) ;
            nTokenLen += nDelimLen ;
            if ( nTokenLen < strUnparsed.length() )
                strUnparsed = strUnparsed.substr( nTokenLen ) ;
            else
                break ;
        }
        else
            break ;
    }

    return nCount ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// add nWords words, each \0 term'd, with extra \0 at end
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t CIVStringSet::Add( LPCTSTR pszzWords, size_t nWords )
{
    size_t nCount = 0 ;

    // Start with the first word in the multi-string
    LPCTSTR pszCurWord = pszzWords ;
    if ( pszCurWord != NULL )
    {
        // stop iterating when we find a "word" beginning with null (the extra \0)
        while ( pszCurWord[0] != _T('\0') )
        {
            if ( Add( pszCurWord ) )  // add it to the FSM and the array
            {
                nCount++ ;

                // Position to the next word, one beyond the null terminator
                size_t nLen = _tcslen( pszCurWord ) ;
                // If this fails, the extra \0 was probably not there, as required
                pszCurWord = &pszzWords[nLen+1] ;
            }
            else
                break ;
        }
    }

#ifdef _DEBUG
    if ( ( nWords != nCount )
      && ( nWords > 0 )
       )
        TRACE("Expected %d words, but found %d!\n", nWords, nCount);
#else
    UNUSED(nWords);
#endif

    return nCount ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// add all the elements of a set of strings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t CIVStringSet::Add( const std::set<_tstring> & sstrWords )
{
    size_t nWord = 0 ;

    // Iterate the string set
    std::set<_tstring>::const_iterator it = sstrWords.begin() ;
    while ( it != sstrWords.end() )
    {
        if ( ! Add( *it++ ) )  // add it to the FSM and the array
            break ;
        nWord++ ;
    }

    return nWord ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// add all the elements of an array of strings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t CIVStringSet::Add( const std::vector<_tstring> & astrWords )
{
    size_t nWord = 0 ;

    // Iterate the string array
    std::vector<_tstring>::const_iterator it = astrWords.begin() ;
    while ( it != astrWords.end() )
    {
        if ( ! Add( *it++ ) )  // add it to the FSM and the array
            break ;
        nWord++ ;
    }

    return nWord ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// add all the elements of a list of strings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t CIVStringSet::Add( const std::list<_tstring> & lstrWords )
{
    size_t nWord = 0 ;

    // Iterate the string list
    std::list<_tstring>::const_iterator it = lstrWords.begin() ;
    while ( it != lstrWords.end() )
    {
        if ( ! Add( *it++ ) )  // add it to the FSM and the array
            break ;
        nWord++ ;
    }

    return nWord ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// InsertWord
//          put the new word into the FSM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool CIVStringSet::InsertWord( LPCTSTR pszWord, unsigned long dwIndex )
{
    bool bRetVal = false ;

    size_t nLen = _tcslen( pszWord ) ;
    if ( m_dwMaxUsedState < MAXDWORD )    // if we've got over 4 billion states, something's wrong
    {
        unsigned long dwCurState = 0UL ;
        for ( UINT nChar = 0U ; nChar < nLen ; nChar++ )
        {
            size_t          nIdxChar = min(static_cast<size_t>(pszWord[nChar]), c_nMaxChar) ;
            SEntry        & rEntry   = m_apnFSM[dwCurState][nIdxChar] ;  // the state, and possibly also an index
            unsigned long   dwState  = rEntry.m_dwState ;
            bool            bNew     = (dwState == 0UL) ;                // no previous words have been here

            if ( bNew )
                dwState = ++m_dwMaxUsedState ;  // use a new state number

            // special case - last character of substring
            if ( nChar == ( nLen-1 ) )
            {
                // Put both the state number and the word index in the entry
                rEntry.m_dwState = dwState ;
                rEntry.m_dwIndex = dwIndex ;
                break ;
            }
            else if ( bNew ) // if current entry for this char value and state is still zero, add to FSM
                rEntry.m_dwState = dwState ;

            dwCurState = dwState ;  // prepare for next iteration
            if ( m_apnFSM.size() <= dwCurState )
                m_apnFSM.resize( ((dwCurState * 2) + 63) & 0xFFC0, std::vector<SEntry>( c_nMaxChar ) ) ;
        }

        bRetVal = true ;
    }

    return bRetVal ;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FindFirstIn
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UINT CIVStringSet::FindFirstIn( const _tstring & strText, size_t & rnFirst )
{
    // The real work is done by FindNext, but we must initialize the starting
    //    character index and string buffer first
    m_nCurTextChar = 0U ;
    m_strSearch = strText ;

    return FindNext( rnFirst ) ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FindNext
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UINT CIVStringSet::FindNext( size_t & rnNext )
{
    unsigned long dwCurState = 0UL ; // start in state 0
    UINT nIdx ;
    UINT nStartChar = MAXDWORD ;
    size_t nLen = m_strSearch.length() ;

    // Start with the character following the last one examined
    for ( nIdx = m_nCurTextChar ; nIdx < nLen ; nIdx++ )
    {
        size_t nNextIdx = m_strSearch.find_first_not_of( m_strDelims, nIdx ) ;
        if ( nNextIdx != nIdx )
        {
            // Skip over delimiters
            nStartChar = MAXDWORD ;
            if ( ( nNextIdx != _tstring::npos )
              && ( nNextIdx < nLen )
               )
            {
                dwCurState = 0UL ;
                nIdx = nNextIdx ;
            }
            else
                break ;
        }

        // if we are in state 0, save the index of the starting character,
        //   in case we must backtrack or for the return value
        if ( dwCurState == 0UL )
            nStartChar = nIdx ;

        // follow up the table states
        size_t nIdxChar = min(static_cast<size_t>(m_strSearch[nIdx]), c_nMaxChar) ;
        const SEntry & rEntry = m_apnFSM[dwCurState][nIdxChar] ;

        // if we reach an entry with a an index, we may have found a full match
        //    if so, return the word we found
        if ( ( rEntry.m_dwIndex != 0UL )
          && ( ( nIdx == nLen-1 )
            || ( m_strDelims.find_first_of( m_strSearch[nIdx+1] ) != _tstring::npos )
             )
           )
        {
            size_t nIndex = rEntry.m_dwIndex - 1 ;
            ASSERT(nIndex < size());
            rnNext = nIndex ;
            m_nCurTextChar = nIdx + 1 ;  // set the index for the next time
            break ;
        }
        else
            dwCurState = rEntry.m_dwState ;

        if ( dwCurState == 0 )
        {
            nStartChar = MAXDWORD ;  // in case we're about to terminate the loop
            nIdx = m_strSearch.find_first_of( m_strDelims, nIdx ) ; // advance to the next delimiter
        }
    }

    // return the index of the start of the word, or -1 if no word found
    return (( nIdx < nLen ) ? nStartChar : MAXDWORD) ;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FindAllIn
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t CIVStringSet::FindAllIn( _tstring strText, CWordPosPairList & rlstrWords )
{
    rlstrWords.clear() ;

    // Iterate through all words, and put them in the list
    size_t nWord = static_cast<size_t>(-1) ;
    UINT nPos = FindFirstIn( strText, nWord ) ;
    if ( nPos != MAXDWORD )
    {
        do
        {
            CWordPosPair posWord( nWord, nPos ) ;
            rlstrWords.push_back( posWord ) ;
            nPos = FindNext( nWord ) ;
        } while ( nPos != MAXDWORD ) ;
    }

    return rlstrWords.size() ;
}
