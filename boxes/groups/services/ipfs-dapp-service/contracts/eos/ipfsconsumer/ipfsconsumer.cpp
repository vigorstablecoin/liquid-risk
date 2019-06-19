#include "../dappservices/ipfs.hpp"
#include "../dappservices/multi_index.hpp"

#define DAPPSERVICES_ACTIONS() \
  XSIGNAL_DAPPSERVICE_ACTION \
  IPFS_DAPPSERVICE_ACTIONS
#define DAPPSERVICE_ACTIONS_COMMANDS() \
  IPFS_SVC_COMMANDS() 

#define CONTRACT_NAME() ipfsconsumer 

CONTRACT_START()
  TABLE testindex {
    uint64_t id;
    uint64_t primary_key()const {return id;}
  };
  TABLE testentry {  
     uint64_t                      field1;
     std::vector<char>             field2;
     uint64_t                      field3;
  };  
  TABLE vconfig {
    uint64_t next_available_key;
    uint32_t shards;
    uint32_t buckets_per_shard; 
  };
  
      
  typedef dapp::multi_index<"test"_n, testindex> testindex_t;
  typedef eosio::multi_index<".vconfig"_n, vconfig> vconfig_t_abi;

  
  [[eosio::action]] void testindex(uint64_t id) {
    testindex_t testset(_self,_self.value);
    testset.emplace(_self, [&]( auto& a ){
      a.id = id;
    });
  }
  [[eosio::action]] void increment() {
    testindex_t testset(_self,_self.value);
    testset.emplace(_self, [&]( auto& a ){
      a.id = testset.available_primary_key();
    });
  }
  [[eosio::action]] void testresize() {
    testindex_t testset(_self,_self.value,512,32);
    testset.emplace(_self, [&]( auto& a ){
      a.id = testset.available_primary_key();
    });
  }
 [[eosio::action]] void testset(testentry data) {
    auto uri = setData(data);
  }
 [[eosio::action]] void testget(std::string uri, uint32_t expectedfield) {
    eosio::check(getData<testentry>(uri).field1 == expectedfield, "wrong value");
  }
 [[eosio::action]] void testempty(std::string uri) {
    eosio::check(getRawData(uri, false, true).size() == 0, "wrong size");
  }  
CONTRACT_END((testset)(testget)(testempty)(increment)(testindex)(testresize))
