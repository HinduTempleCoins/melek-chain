#pragma once
#include <steem/chain/steem_object_types.hpp>

#include <fc/array.hpp>

namespace steem { namespace plugins { namespace evm {

using namespace steem::chain;

//
// MELEK EVM state — chainbase objects at plugin SPACE_ID 21.
//
// SPACE_IDs in use: 16 (rc), 20 (token_emissions). 21 is free, so the core
// steem_object_types.hpp `object_type` enum is NOT touched during the plugin/PoC
// phase. All EVM state lives here as ordinary chainbase objects, so the existing
// undo / fork / hashing machinery covers EVM rollback for free (never an external
// LevelDB/trie — the checkpoint root is a *derived* commitment, not the store).
//
// P0: the indexes are registered but stay EMPTY (no evaluator writes to them until
// the P1 evmone-backed host lands). 256-bit words are stored as 32 big-endian bytes;
// all EVM arithmetic happens through intx in P1, never on these raw byte arrays.
//
#ifndef STEEM_EVM_SPACE_ID
#define STEEM_EVM_SPACE_ID 21
#endif

typedef fc::array< char, 20 > evm_address;
typedef fc::array< char, 32 > evm_word;

enum evm_object_type
{
   evm_account_object_type = ( STEEM_EVM_SPACE_ID << 8 ),
   evm_storage_object_type = ( STEEM_EVM_SPACE_ID << 8 ) + 1,
   evm_code_object_type    = ( STEEM_EVM_SPACE_ID << 8 ) + 2
};

class evm_account_object : public object< evm_account_object_type, evm_account_object >
{
public:
   template< typename Constructor, typename Allocator >
   evm_account_object( Constructor&& c, allocator< Allocator > a )
   {
      c( *this );
   }

   evm_account_object() {}

   id_type       id;
   evm_address   address;
   evm_word      balance;      ///< wei, 32 big-endian bytes
   uint64_t      nonce = 0;
   evm_word      code_hash;    ///< keccak256(code); zero for EOAs
};

class evm_storage_object : public object< evm_storage_object_type, evm_storage_object >
{
public:
   template< typename Constructor, typename Allocator >
   evm_storage_object( Constructor&& c, allocator< Allocator > a )
   {
      c( *this );
   }

   evm_storage_object() {}

   id_type       id;
   evm_address   address;      ///< owning contract
   evm_word      key;          ///< 32-byte slot key
   evm_word      value;        ///< 32-byte slot value
};

class evm_code_object : public object< evm_code_object_type, evm_code_object >
{
public:
   template< typename Constructor, typename Allocator >
   evm_code_object( Constructor&& c, allocator< Allocator > a )
      : code( a )
   {
      c( *this );
   }

   id_type       id;
   evm_word      code_hash;    ///< keccak256(code) — dedup key
   shared_string code;         ///< raw deployed bytecode
};

typedef oid< evm_account_object > evm_account_id_type;
typedef oid< evm_storage_object > evm_storage_id_type;
typedef oid< evm_code_object >    evm_code_id_type;

struct by_address;
struct by_address_key;
struct by_code_hash;

typedef multi_index_container<
   evm_account_object,
   indexed_by<
      ordered_unique< tag< by_id >,      member< evm_account_object, evm_account_id_type, &evm_account_object::id > >,
      ordered_unique< tag< by_address >, member< evm_account_object, evm_address, &evm_account_object::address > >
   >,
   allocator< evm_account_object >
> evm_account_index;

typedef multi_index_container<
   evm_storage_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< evm_storage_object, evm_storage_id_type, &evm_storage_object::id > >,
      ordered_unique< tag< by_address_key >,
         composite_key< evm_storage_object,
            member< evm_storage_object, evm_address, &evm_storage_object::address >,
            member< evm_storage_object, evm_word,    &evm_storage_object::key >
         >,
         composite_key_compare< std::less< evm_address >, std::less< evm_word > >
      >
   >,
   allocator< evm_storage_object >
> evm_storage_index;

typedef multi_index_container<
   evm_code_object,
   indexed_by<
      ordered_unique< tag< by_id >,        member< evm_code_object, evm_code_id_type, &evm_code_object::id > >,
      ordered_unique< tag< by_code_hash >, member< evm_code_object, evm_word, &evm_code_object::code_hash > >
   >,
   allocator< evm_code_object >
> evm_code_index;

} } } // steem::plugins::evm

FC_REFLECT( steem::plugins::evm::evm_account_object, (id)(address)(balance)(nonce)(code_hash) )
CHAINBASE_SET_INDEX_TYPE( steem::plugins::evm::evm_account_object, steem::plugins::evm::evm_account_index )

FC_REFLECT( steem::plugins::evm::evm_storage_object, (id)(address)(key)(value) )
CHAINBASE_SET_INDEX_TYPE( steem::plugins::evm::evm_storage_object, steem::plugins::evm::evm_storage_index )

FC_REFLECT( steem::plugins::evm::evm_code_object, (id)(code_hash)(code) )
CHAINBASE_SET_INDEX_TYPE( steem::plugins::evm::evm_code_object, steem::plugins::evm::evm_code_index )
