#include <steem/protocol/evm_operations.hpp>
#include <steem/protocol/validation.hpp>

namespace steem { namespace protocol {

// P0 note: these are lightweight structural bounds only. The real gas/value/precision
// checks (and the wei<->MELEK bridge) land with the P1 evmone-backed evaluator bodies.
// PoC caps keep a reserved-but-unwired op from being able to carry unbounded payloads.
#define MELEK_EVM_MAX_CALLDATA_BYTES   (128 * 1024)   // 128 KiB PoC input cap
#define MELEK_EVM_MAX_GAS_LIMIT        (uint64_t( 30000000 ))  // ~Ethereum block gas cap

void evm_deposit_operation::validate()const
{
   validate_account_name( from );
   FC_ASSERT( amount.amount > 0, "evm_deposit amount must be positive" );
   FC_ASSERT( amount.symbol == STEEM_SYMBOL, "evm_deposit only bridges the native MELEK asset" );
}

void evm_withdraw_operation::validate()const
{
   validate_account_name( to );
   FC_ASSERT( amount.amount > 0, "evm_withdraw amount must be positive" );
   FC_ASSERT( amount.symbol == STEEM_SYMBOL, "evm_withdraw only bridges the native MELEK asset" );
}

void evm_call_operation::validate()const
{
   validate_account_name( payer );
   FC_ASSERT( gas_limit > 0 && gas_limit <= MELEK_EVM_MAX_GAS_LIMIT, "evm_call gas_limit out of range" );
   FC_ASSERT( input.size() <= MELEK_EVM_MAX_CALLDATA_BYTES, "evm_call input exceeds PoC size cap" );
}

} } // steem::protocol
