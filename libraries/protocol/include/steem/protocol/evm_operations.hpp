#pragma once
#include <steem/protocol/base.hpp>
#include <steem/protocol/asset.hpp>

#include <fc/array.hpp>

namespace steem { namespace protocol {

   /**
    * Native EVM operations (Route B). RESERVED in P0 (HF 0.25) — the on-chain
    * serialization tags are frozen here so the eventual trustless flip to
    * base-consensus EVM dispatch is a *behavior* hardfork, not a state migration.
    *
    * In P0 every evaluator body is an HF-gated no-op (FC_ASSERT has_hardfork 0.25).
    * The evmone-backed bodies land in P1 behind these exact tags. Do NOT reorder
    * or remove any field — these are consensus-serialized ordinals.
    *
    * An EVM address is 20 raw bytes; a 256-bit word is 32 big-endian bytes.
    */
   typedef fc::array< char, 20 > evm_address_type;
   typedef fc::array< char, 32 > evm_word_type;

   /**
    * Move value from a Graphene account into the parallel EVM address space.
    * Precision bridge (10^18 wei = 1 MELEK) is applied by the P1 evaluator.
    */
   struct evm_deposit_operation : public base_operation
   {
      account_name_type   from;           ///< Graphene payer
      evm_address_type    to;             ///< destination EVM address
      asset               amount;         ///< MELEK to credit (converted to wei in P1)

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( from ); }
   };

   /**
    * Move value from the EVM address space back to a Graphene account.
    * Sub-satoshi remainders are rejected by the P1 evaluator (dust stays in EVM).
    */
   struct evm_withdraw_operation : public base_operation
   {
      account_name_type   to;             ///< Graphene recipient (also the authorizing account)
      evm_address_type    from;           ///< source EVM address
      asset               amount;         ///< MELEK to release

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( to ); }
   };

   /**
    * Deploy (empty `to`) or call an EVM contract. `input` is the ABI-encoded
    * calldata (or init code on deploy). `value` is a 32-byte big-endian wei amount.
    */
   struct evm_call_operation : public base_operation
   {
      account_name_type   payer;          ///< Graphene account that pays RC
      evm_address_type    caller;         ///< EVM sender address
      bool                is_create = false;
      evm_address_type    to;             ///< target contract (ignored when is_create)
      evm_word_type       value;          ///< wei, 32 big-endian bytes
      uint64_t            gas_limit = 0;
      std::vector< char > input;          ///< calldata / init code

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( payer ); }
   };

} } // steem::protocol

FC_REFLECT( steem::protocol::evm_deposit_operation,  (from)(to)(amount) )
FC_REFLECT( steem::protocol::evm_withdraw_operation, (to)(from)(amount) )
FC_REFLECT( steem::protocol::evm_call_operation,     (payer)(caller)(is_create)(to)(value)(gas_limit)(input) )
