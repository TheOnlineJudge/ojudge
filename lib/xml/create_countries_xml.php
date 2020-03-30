#!/usr/bin/php

<?php

	// Country list and flags from: https://github.com/hjnilsson/country-flags
	// Use this PHP script to generate the XML file

	echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	echo "<messages>\n";

	$countries = json_decode(file_get_contents('countries.json'));

	foreach($countries as $key => $val) {
		printf("<message id=\"country_name_%s\">%s</message>\n",strtolower($key),$val);
		printf("<message id=\"country_flag_%s\">%s</message>\n",strtolower($key),file_get_contents("svg/".strtolower($key).".svg"));
	}

	echo "</messages>\n";

?>
