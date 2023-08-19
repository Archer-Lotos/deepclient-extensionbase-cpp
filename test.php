<?php

class DeepClient {
    private static $_ids = array();
    private static $_serialize = array();
    private static $token;
    private static $url;

    public function select($exp, $options = array()) {
        return array(
            "error" => array("message" => "!exp"),
            "data" => null,
            "loading" => false,
            "networkStatus" => null
        );
    }

    public function insert($objects, $options = array()) {
        return array(
            "error" => array("message" => "!record"),
            "data" => null,
            "loading" => false,
            "networkStatus" => null
        );
    }

    public function update($exp, $value, $options = array()) {
        return array(
            "error" => array("message" => "!exp or !value"),
            "data" => null,
            "loading" => false,
            "networkStatus" => null
        );
    }

    public function delete($exp, $options = array()) {
        return array();
    }

    public function serial($asyncSerialParams) {
        return array("error" => "test");
    }

    public function reserve() {
        throw new Exception("Method not implemented");
    }

    public function wait_for() {
        throw new Exception("Method not implemented");
    }

    public function id($start, ...$path) {
        return 1;
    }

    public function id_local() {
        throw new Exception("Method not implemented");
    }

    public function guest() {
        throw new Exception("Method not implemented");
    }

    public function jwt() {
        throw new Exception("Method not implemented");
    }

    public function whoami() {
        throw new Exception("Method not implemented");
    }

    public function login() {
        throw new Exception("Method not implemented");
    }

    public function logout() {
        throw new Exception("Method not implemented");
    }

    public function can() {
        throw new Exception("Method not implemented");
    }

    public function name() {
        throw new Exception("Method not implemented");
    }

    public function name_local() {
        throw new Exception("Method not implemented");
    }
}

/*
function php_deep_client_select() {
    $deep_client = make_deep_client(
    'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJodHRwczovL2hhc3VyYS5pby9qd3QvY2xhaW1zIjp7IngtaGFzdXJhLWFsbG93ZWQtcm9sZXMiOlsiYWRtaW4iXSwieC1oYXN1cmEtZGVmYXVsdC1yb2xlIjoiYWRtaW4iLCJ4LWhhc3VyYS11c2VyLWlkIjoiMzgwIn0sImlhdCI6MTY5MTkxMTQxM30.W0GOuqOvRZrgrVZkLaceKTPBitXwR-1WlxLgxUZXOnY',
    'http://localhost:3006/gql'
    );

    return $deep_client->select(1);
}

var_dump(php_deep_client_select());
*/


extension_loaded('deep_client_php_extension') or dl('deep_client_php_extension.so');


$client = new DeepClientPhpWrapper(
	'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJodHRwczovL2hhc3VyYS5pby9qd3QvY2xhaW1zIjp7IngtaGFzdXJhLWFsbG93ZWQtcm9sZXMiOlsiYWRtaW4iXSwieC1oYXN1cmEtZGVmYXVsdC1yb2xlIjoiYWRtaW4iLCJ4LWhhc3VyYS11c2VyLWlkIjoiMzgwIn0sImlhdCI6MTY5MTkxMTQxM30.W0GOuqOvRZrgrVZkLaceKTPBitXwR-1WlxLgxUZXOnY',
	'http://localhost:3006/gql'
);
var_dump($client);
var_dump($client->select(1));
var_dump($client->getPyObject("1", "2"));
//var_dump($client->insert("sql query here"));

?>