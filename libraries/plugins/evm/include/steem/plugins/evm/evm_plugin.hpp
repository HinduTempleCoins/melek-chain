#pragma once
#include <steem/chain/steem_fwd.hpp>
#include <steem/plugins/chain/chain_plugin.hpp>

#include <appbase/application.hpp>

namespace steem { namespace plugins { namespace evm {

#define STEEM_EVM_PLUGIN_NAME "evm"

namespace detail { class evm_plugin_impl; }

//
// MELEK native-EVM plugin (P0 skeleton).
//
// P0 responsibility: register the SPACE_ID-21 EVM state indexes (evm_account /
// evm_storage / evm_code) so the consensus surface reserved at HF 0.25 is materialized,
// and install a no-op post-apply-block handler where the P2 checkpoint push will live.
//
// ZERO evmone dependency. No Route-A custom_json interpreter and no host are wired yet —
// those land in P1 (evmone + melek_evm_host + generic_custom_operation_interpreter).
//
class evm_plugin : public appbase::plugin< evm_plugin >
{
   public:
      evm_plugin();
      virtual ~evm_plugin();

      APPBASE_PLUGIN_REQUIRES( (steem::plugins::chain::chain_plugin) )

      static const std::string& name() { static std::string name = STEEM_EVM_PLUGIN_NAME; return name; }

      virtual void set_program_options( boost::program_options::options_description& cli, boost::program_options::options_description& cfg ) override;
      virtual void plugin_initialize( const boost::program_options::variables_map& options ) override;
      virtual void plugin_startup() override;
      virtual void plugin_shutdown() override;

   private:
      std::unique_ptr< detail::evm_plugin_impl > my;
};

} } } // steem::plugins::evm
