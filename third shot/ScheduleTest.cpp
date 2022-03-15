#include <hpx/hpx_init.hpp>
#include <hpx/iostream.hpp>
#include <hpx/future.hpp>
#include <hpx/runtime_distributed/find_all_localities.hpp>
#include <boost/range/irange.hpp>

//#include <hpx/hpx_main.hpp>


/*
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <numeric>
*/

/// tested on ubuntu 20.04.03 (singularity/docker)
/// hpx master or 1.7.1 compiled as release with clang 12
/// started on a AMD Ryzen 9 5950X with
///   srun -n 16 -N 1 -w "MYNODE" ./scheduletest

class Component : public hpx::components::component_base<Component> {
   public:
      Component() = default;

      auto execute() const -> std::size_t {
         auto f = []() {
            std::vector<unsigned int> grid( 99, 0u );
            for( auto i( 0u ); i < 99999u; ++i ) {
               for( auto j( 0u ); j < 25; ++j ){
                  grid[j] += 1u;
                  grid[j+75] += 1u;
               }
            }
         };
         f();
         //hpx::async( hpx::launch::async, f );
         return hpx::get_locality_id();
      }

      HPX_DEFINE_COMPONENT_ACTION( Component, execute );
};

HPX_REGISTER_COMPONENT( hpx::components::component<Component>, Component );
HPX_REGISTER_ACTION( Component::execute_action );


class ComponentClient : public hpx::components::client_base<ComponentClient, Component> {
      using BaseType = hpx::components::client_base<ComponentClient, Component>;

   public:
      template<typename ...Arguments>
      ComponentClient( Arguments ...arguments )
         : BaseType( std::move( arguments )... ) {
      }

      template<typename ...Arguments>
      hpx::future<std::size_t> execute( Arguments ...arguments ) {
         return hpx::async<Component::execute_action>( this->get_id(), std::move( arguments )... );
      }
};


int hpx_main() {
   static constexpr auto executeperloc = 4u;

   std::vector<ComponentClient> clients;
   auto localities( hpx::find_all_localities() );
   std::transform( std::begin( localities ), std::end( localities ),
                   std::back_inserter( clients ),
                   []( auto& loc ) {
                      return hpx::new_<ComponentClient>( loc );
                   } );

   std::vector<hpx::future<std::size_t>> results;
   for( auto clieentindex : boost::irange( 0ul, clients.size() ) ) {
      for( auto unused : boost::irange( 0u, executeperloc ) ) {
         results.emplace_back( clients[clieentindex].execute() );
      }
   }

   for( auto counter( results.size() ); counter < 99999ul; ) {
      hpx::wait_any( results );
      for( auto&& res: results ) {
         if( res.is_ready() ) {
            if( res.has_value() ) {
               auto result( res.get() );
               res = clients[result].execute();
               hpx::cout << "Shot " << counter++ << " on " << result << std::endl;
            } else {
               hpx::cerr << "Error shot " << counter;
            }
         }
      }
   }

   hpx::wait_all( results );
   return hpx::finalize(); 
}


int main( int argc, char* argv[] ) {
	return hpx::init( argc, argv );
}
