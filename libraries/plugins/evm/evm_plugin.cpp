#include <steem/chain/steem_fwd.hpp>

#include <steem/plugins/evm/evm_plugin.hpp>
#include <steem/plugins/evm/evm_objects.hpp>

#include <steem/chain/database.hpp>
#include <steem/chain/index.hpp>
#include <steem/chain/util/signal.hpp>

namespace steem { namespace plugins { namespace evm {

namespace detail {

class evm_plugin_impl
{
public:
   evm_plugin_impl() : _db( appbase::app().get_plugin< steem::plugins::chain::chain_plugin >().db() ) {}
   virtual ~evm_plugin_impl() {}

   // P0: reserved hook where the P2 EVM state-checkpoint required-action will be pushed
   // (recompute the roots over this block's dirty set, then push_required_action). No-op now.
   void on_post_apply_block( const chain::block_notification& note ) {}

   chain::database&              _db;
   boost::signals2::connection   post_apply_block_connection;
};

} // detail

evm_plugin::evm_plugin() {}
evm_plugin::~evm_plugin() {}

void evm_plugin::set_program_options( boost::program_options::options_description& cli, boost::program_options::options_description& cfg ) {}

void evm_plugin::plugin_initialize( const boost::program_options::variables_map& options )
{
   try
   {
      ilog( "evm: plugin_initialize() begin (P0 surface freeze — no evmone, no execution)" );

      my = std::make_unique< detail::evm_plugin_impl >();

      // Register the SPACE_ID-21 EVM state indexes. They stay EMPTY in P0 — no evaluator
      // writes to them until the P1 evmone-backed host is wired.
      STEEM_ADD_PLUGIN_INDEX( my->_db, evm_account_index );
      STEEM_ADD_PLUGIN_INDEX( my->_db, evm_storage_index );
      STEEM_ADD_PLUGIN_INDEX( my->_db, evm_code_index );

      my->post_apply_block_connection = my->_db.add_post_apply_block_handler(
         [&]( const chain::block_notification& note )
         {
            try
            {
               my->on_post_apply_block( note );
            } FC_LOG_AND_RETHROW()
         }, *this, 0 );

      ilog( "evm: plugin_initialize() end" );
   } FC_CAPTURE_AND_RETHROW()
}

void evm_plugin::plugin_startup() {}

void evm_plugin::plugin_shutdown()
{
   chain::util::disconnect_signal( my->post_apply_block_connection );
}

} } } // steem::plugins::evm
