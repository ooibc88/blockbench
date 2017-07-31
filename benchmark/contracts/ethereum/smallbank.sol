pragma solidity ^0.4.0;

contract KVstore {

  mapping(string=>string) store;

  function get(string key) constant returns(string) {
    return store[key];
  }
  function set(string key, string value) {
    store[key] = value;

    bytes memory bkey = bytes(key);
    bkey[0] = 'a';
    string memory skey = string(bkey);
    string memory rkey = store[key];
    store[skey] = value;

    bkey[1] = 'b';
    skey = string(bkey);
    rkey = store[skey];
    store[skey] = value;

    bkey[1] = 'c';
    skey = string(bkey);
    rkey = store[skey];
    store[skey] = value;

    bkey[1] = 'd';
    skey = string(bkey);
    rkey = store[skey];
    store[skey] = value;
  }
}
