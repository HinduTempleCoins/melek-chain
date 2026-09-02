#include <steem/chain/steem_fwd.hpp>
#include <steem/chain/steem_evaluator.hpp>
#include <steem/chain/database.hpp>

#include <steem/protocol/evm_operations.hpp>

namespace steem { namespace chain {

// ============================================================================
// MELEK native EVM operation evaluators (Route B) — P0 reservation.
//
// These bodies are HF-gated NO-OPS. They exist only so the native evm_* op tags
// (frozen at operations.hpp) have registered evaluators and the tree compiles with
// ZERO evmone dependency. Until HF 0.25 activates they FC_ASSERT-reject, and even
// after activation the P0 body does nothing — the evmone-backed do_apply (op ->
// evmc_message -> melek_evm_host -> chainbase SPACE_ID-21 objects) lands in P1
// behind these exact tags, making the eventual turn-on a behavior change only.
// ============================================================================

void evm_deposit_evaluator::do_apply( const evm_deposit_operation& o )
{
   FC_ASSERT( _db.has_hardfork( STEEM_HARDFORK_0_25 ), "Native EVM operations are not enabled until HF 0.25" );
   FC_ASSERT( false, "MELEK native EVM is reserved but not yet implemented (P0 no-op)" );
}

void evm_withdraw_evaluator::do_apply( const evm_withdraw_operation& o )
{
   FC_ASSERT( _db.has_hardfork( STEEM_HARDFORK_0_25 ), "Native EVM operations are not enabled until HF 0.25" );
   FC_ASSERT( false, "MELEK native EVM is reserved but not yet implemented (P0 no-op)" );
}

void evm_call_evaluator::do_apply( const evm_call_operation& o )
{
   FC_ASSERT( _db.has_hardfork( STEEM_HARDFORK_0_25 ), "Native EVM operations are not enabled until HF 0.25" );
   FC_ASSERT( false, "MELEK native EVM is reserved but not yet implemented (P0 no-op)" );
}

} } // steem::chain
