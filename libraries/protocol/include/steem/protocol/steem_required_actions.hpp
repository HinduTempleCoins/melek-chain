#pragma once
#include <steem/protocol/types.hpp>
#include <steem/protocol/base.hpp>
#include <steem/protocol/asset.hpp>
#include <steem/protocol/misc_utilities.hpp>

#include <fc/crypto/sha256.hpp>

namespace steem { namespace protocol {

#ifdef IS_TEST_NET
   struct example_required_action : public base_operation
   {
      account_name_type account;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }

      friend bool operator==( const example_required_action& lhs, const example_required_action& rhs );
   };
#endif

   struct smt_refund_action : public base_operation
   {
      account_name_type contributor;
      asset_symbol_type symbol;
      uint32_t          contribution_id;
      asset             refund;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( contributor ); }

      friend bool operator==( const smt_refund_action& lhs, const smt_refund_action& rhs );
   };

   struct contribution_payout
   {
      asset payout;
      bool  to_vesting;

      friend bool operator==( const contribution_payout& rhs, const contribution_payout& lhs );
   };

   struct smt_contributor_payout_action : public base_operation
   {
      account_name_type    contributor;
      asset_symbol_type    symbol;
      uint32_t             contribution_id;
      asset                contribution;
      std::vector< contribution_payout > payouts;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( contributor ); }

      friend bool operator==( const smt_contributor_payout_action& lhs, const smt_contributor_payout_action& rhs );
   };

   struct smt_founder_payout_action : public base_operation
   {
      asset_symbol_type                                   symbol;
      std::map< account_name_type, std::vector< contribution_payout > > account_payouts;
      share_type                                          market_maker_steem  = 0;
      share_type                                          market_maker_tokens = 0;
      share_type                                          reward_balance      = 0;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const
      {
         for ( auto& entry : account_payouts )
            a.insert( entry.first );
      }

      friend bool operator==( const smt_founder_payout_action& lhs, const smt_founder_payout_action& rhs );
   };

   struct smt_ico_launch_action : public base_operation
   {
      account_name_type control_account;
      asset_symbol_type symbol;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( control_account ); }

      friend bool operator==( const smt_ico_launch_action& lhs, const smt_ico_launch_action& rhs );
   };

   struct smt_ico_evaluation_action : public base_operation
   {
      account_name_type control_account;
      asset_symbol_type symbol;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( control_account ); }

      friend bool operator==( const smt_ico_evaluation_action& lhs, const smt_ico_evaluation_action& rhs );
   };

   struct smt_token_launch_action : public base_operation
   {
      account_name_type control_account;
      asset_symbol_type symbol;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( control_account ); }

      friend bool operator==( const smt_token_launch_action& lhs, const smt_token_launch_action& rhs );
   };

   /**
    * MELEK EVM state checkpoint (Design C spine). RESERVED at HF 0.25.
    *
    * A chain-generated required action: the EVM plugin recomputes the EVM state /
    * receipts roots over each block's dirty set and pushes this action; every witness
    * re-derives it and process_required_actions FC_ASSERTs byte-equality, so a host /
    * precompile divergence REJECTS the block instead of splitting the chain silently.
    *
    * In P0 the plugin never pushes it and the evaluator is a no-op — this only freezes
    * the required_automated_action serialization tag. System-generated, no signing
    * authority (inherits base_operation's empty authority set).
    */
   struct evm_state_checkpoint_action : public base_operation
   {
      uint32_t     block_num = 0;
      fc::sha256   evm_state_root;
      fc::sha256   evm_receipts_root;
      fc::sha256   prev_checkpoint;
      uint32_t     evm_tx_count = 0;

      void validate()const;

      friend bool operator==( const evm_state_checkpoint_action& lhs, const evm_state_checkpoint_action& rhs );
   };
} } // steem::protocol

#ifdef IS_TEST_NET
FC_REFLECT( steem::protocol::example_required_action, (account) )
#endif

FC_REFLECT( steem::protocol::smt_refund_action, (contributor)(symbol)(contribution_id)(refund) )
FC_REFLECT( steem::protocol::contribution_payout, (payout)(to_vesting) )
FC_REFLECT( steem::protocol::smt_contributor_payout_action, (contributor)(symbol)(contribution_id)(contribution)(payouts) )
FC_REFLECT( steem::protocol::smt_founder_payout_action, (symbol)(account_payouts)(market_maker_steem)(market_maker_tokens)(reward_balance) )
FC_REFLECT( steem::protocol::smt_ico_launch_action, (control_account)(symbol) )
FC_REFLECT( steem::protocol::smt_ico_evaluation_action, (control_account)(symbol) )
FC_REFLECT( steem::protocol::smt_token_launch_action, (control_account)(symbol) )
FC_REFLECT( steem::protocol::evm_state_checkpoint_action, (block_num)(evm_state_root)(evm_receipts_root)(prev_checkpoint)(evm_tx_count) )
