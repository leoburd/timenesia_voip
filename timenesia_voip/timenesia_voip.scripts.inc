<?php


/**
 * Implementation of hook_voipscript_get_script_names()
 */
function timenesia_voip_voipscript_get_script_names() {
  $script_names[] = 'timenesia_welcome';
  $script_names[] = 'timenesia_play_destination_script';
  $script_names[] = 'timenesia_destination_script';
  $script_names[] = 'timenesia_record_options_script';
  $script_names[] = 'timenesia_record_memory_script';
  $script_names[] = 'timenesia_play_memories_script';
  $script_names[] = 'timenesia_sms_handler_script';
  return $script_names;
}

/**
 * Implementation of hook_voipscript_load_script()
 */
function timenesia_voip_voipscript_load_script($script_name, $options=NULL) {

  require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'timenesia_voip.prompts.inc');

  $script = NULL;
  switch($script_name) {
    default:
      break;

    case 'timenesia_welcome':
      $options['language'] = 'English';

      $script = new VoipScript('timenesia_welcome');
      $script->addSetVoice('woman');
      $script->addSet('max_session_duration', 0);

      // get the prompt messages from the main neighborhood of the system
      $eid = variable_get('timenesia_voip_neighborhood_eid', 0);
      $extension = voipextension_load($eid);
      $node = node_load($extension['module_id']);
      $system_greeting = voipnode_get_greeting($node);
      $system_description = voipnode_get_description($node);

      $script->addSay($system_greeting);

      // read destination id
      $script->addSet('counter', 0); // number of attemtps

      $script->addLabel('system_options');
      $script->addGetInput(v('p_system_menu'), NULL, '#', 3);

      $script->addSet('destination_id', '%input_digits');
      $script->addGotoIf('no_input_received', "^%input_digits == ''");
      $script->addGotoIf('browse_destinations', "^%input_digits == '0'");
      $script->addGotoIf('learn_more', "^%input_digits == '*'");
      $script->addGoto('play_destination');

      $script->addLabel('no_input_received');
      $script->addSet('counter', '^%counter + 1');
      $script->addGotoIf('go_back', '^%counter > 3');
      $script->addSay(v('p_try_again'));
      $script->addGoto('system_options');

      $script->addLabel('play_destination');
      $options['destination_id'] = '%destination_id';
      $script->addGosub('timenesia_play_destination_script', $options);
      $script->addGoto('system_options');

      $script->addLabel('browse_destinations');
      $options['module'] = 'voipnode';
      $options['extension_type'] = 'destination';
      $options['p_directory_name'] = v('p_directory_name');
      $script->addGosub('voipextension_directory', $options);
      $script->addSet('counter', 0);
      $script->addGoto('system_options');

      $script->addLabel('learn_more');
      $script->addSet('counter', 0);
      $input_options = array(
        '1' => 'learn_more',
        '#' => 'system_options',
        'i' => 'system_options',
        't' => 'system_options'
      );
      $system_description_menu = v('p_system_description_menu');
      $script->addRunIvrMenu(" $system_description $system_description_menu ", $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addSay(v('p_thank_you'));
      $script->addHangup();

      break;

    case 'timenesia_play_destination_script':
      $script = new VoipScript('timenesia_play_destination_script');
      $eid = $options['destination_id'];
      $extension = voipextension_load($eid);
      if (!$extension) {
        $script->addSay(v('p_invalid_location'));
        $script->addSet('invalid_extension_number', TRUE);
      }
      else {
        $script->addSay(v('p_transferring_to_location') . $eid . ' .');
        $script->addSet('invalid_extension_number', FALSE);
        // run the script associated with the extension
        $gosub = voipextension_get_script($extension);
        $options = $gosub['script_arguments'];
        $options['eid'] = $eid;
        $script->addGosub($gosub['script_name'], $options);
      }
      break;

    case 'timenesia_destination_script':
      $extension = voipextension_load($options['eid']);
      $destination_nid = $extension['module_id'];
      $node = node_load($destination_nid);
      $description = voipnode_get_description($node);
      $greeting = voipnode_get_greeting($node);

      $script = new VoipScript('timenesia_destination_script');
      $script->addSay($greeting);

      $script->addLabel('destination_options');
      $input_options = array(
        '1' => 'learn_more',
        '2' => 'record_memory',
        '3' => 'listen_to_memories',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu(v('p_destination_menu'), $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('learn_more');
      $script->addSay($description);
      $script->addGoto('destination_options');

      $script->addLabel('record_memory');
      $options['destination_nid'] = $destination_nid;
      $script->addGosub('timenesia_record_options_script', $options);
      $script->addGoto('destination_options');

      $script->addLabel('listen_to_memories');
      $options['destination_nid'] = $destination_nid;
      $script->addGosub('timenesia_play_memories_script', $options);
      $script->addGoto('destination_options');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');

      break;

    case 'timenesia_record_options_script':
      $script = new VoipScript('timenesia_record_options_script');

      $script->addLabel('recording_menu');
      $prompt = v('p_recording_menu');
      $input_options = array(
        '1' => 'past',
        '2' => 'present',
        '3' => 'future',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($prompt, $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('past');
      $script->addSay(v('p_record_past'));
      $options['memory_type'] = 'past';
      $script->addGosub('timenesia_record_memory_script', $options);
      $script->addGoto('go_back');

      $script->addLabel('present');
      $script->addSay(v('p_record_present'));
      $options['memory_type'] = 'present';
      $script->addGosub('timenesia_record_memory_script', $options);
      $script->addGoto('go_back');

      $script->addLabel('future');
      $script->addSay(v('p_record_future'));
      $options['memory_type'] = 'future';
      $script->addGosub('timenesia_record_memory_script', $options);
      $script->addGoto('go_back');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();

      break;

    case 'timenesia_record_memory_script':
      $script = new VoipScript('timenesia_record_memory_script');

      $script->addLabel('start');
      $prompt = v('p_recording_prompt');
      $timeout = 5;
      $end_key = '#';
      $max_length = AUDIOBLOG_MAX_STORY_DURATION;
      $script->addRecord($prompt, $timeout, $end_key, $max_length);

      $script->addLabel('accept_menu');
      $prompt = v('p_recording_confirmation'); 
      $input_options = array(
        '1' => 'play_recording',
        '2' => 'accept_recording',
        '*' => 'start',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($prompt, $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('play_recording');
      $script->addSay(v('p_recording_playback'));
      $script->addSay('%recording_url');
      $script->addGoto('accept_menu');

      $script->addLabel('accept_recording');
      $serialized_options = serialize($options);
      $destination_nid = $options['destination_nid'];
      $script->addSet('callback_result',
        "^_timenesia_voip_recording_callback($destination_nid, %call_id, %recording_url, %recording_duration, '$serialized_options')");
// TODO: check callback result
      $script->addSay(v('p_story_accepted'));
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
      break;

    case 'timenesia_play_memories_script':
      $script = _timenesia_voip_get_play_memories_script($options['destination_nid']);
      break;

    case 'timenesia_sms_handler_script':
      $script = new VoipScript('timenesia_sms_handler_script');
      // below was the only way to avoid problems with special characters in the text message
      $options['text'] = '%inbound_text_contents';
      $options['call_id'] = '%call_id';
      $script->addGosub('timenesia_sms_process_request_script', $options);
      break;

    case 'timenesia_sms_process_request_script':
      $script = new VoipScript('timenesia_sms_process_request_script');
      $result = _timenesia_sms_process_request($options['text'], $options['call_id']);
      $script->addSendText($result);
      $script->addHangup();
      break;
  }

  return $script;
}


/**
 * Internal functions
 */

function _timenesia_voip_recording_callback($destination_nid, $call_id, $recording_url, $recording_duration, $serialized_options) {
  $audioblog_options = unserialize($serialized_options);
  $audioblog_options['node_type'] = 'memory';
  $audioblog_options['audio_field_name'] = 'field_memory_audio';
  $audioblog_options['audioblog_type'] = $audioblog_options['memory_type'];
  $audioblog_options['memory_destination'] = $destination_nid;
  $serialized_options = serialize($audioblog_options);
  $rc = audioblog_recording_callback($call_id, $recording_url, $recording_duration, $serialized_options);
  return $rc;
}

function _timenesia_voip_get_play_memories_script($destination_nid) {
  $dest_node = node_load($destination_nid);
  $title = $node->title;
  $destination_name = voipnode_get_title($dest_node);

  $script = new VoipScript("memories for destination $destination_nid");

  $memories = _timenesia_voip_get_memories_from_destination_nid($destination_nid);

  if(!$memories) {
    $p = t('We are sorry, but there are currently no stories recorded for @destination_name . ', array('@destination_name' => $destination_name));
    $script->addSay($p);
    $script->addGoto('go_back');
  }
  else {
    foreach($memories as $memory) {
      $p = t('Story about the @when recorded by @name. ', array('@when' => $memory['when'], '@name' => $memory['author']));
      $script->addSay($p);
      $script->addGetInput($memory['audio_url'], 1, '', 3);
$next_memory_options = v('p_next_memory_menu');
$next_memory_menu = t('@next_memory_menu @destination_name. ', array('@next_memory_menu' => $next_memory_options, '@destination_name' => $destination_name));
      $script->addGetInput($next_memory_menu, 1, '', 3);
      $script->addGotoIf('go_back', '^%input_digits == "#"');
    }
$no_more = v('p_no_more_memories_to_play');
$no_more_memories_to_play = t('@no_more @destination_name', array('@no_more' => $no_more, '@destination_name' => $destination_name));
    $script->addSay("$no_more_memories_to_play");
  }

  $script->addLabel('go_back');

  return $script;
}

function _timenesia_voip_get_memories_from_destination_nid($destination_nid) {
  $memories = array();
  $sql = "SELECT n.nid AS nid FROM {node} n LEFT JOIN {content_type_memory} memory ON n.vid = memory.vid WHERE (n.status <> 0) AND (n.type = 'memory') AND (memory.field_memory_destination_nid = '$destination_nid')";
  $result = db_query($sql);
  while($o = db_fetch_object($result)) {
    $n = node_load($o->nid);
    if($n->field_memory_audio[0]) {
      $name = ($n->name) ? $n->name : t('Anonymous');
      $audio_url = url($n->field_memory_audio[0]['filepath'], array('absolute' => TRUE));
      $memories[$n->changed] = array(
        'nid' => $n->nid,
        'author' => $name,
        'audio_url' => $audio_url,
        'when' => _audioblog_get_category($n, 'When')
      );
    }
  }
  // sort array in reverse chronological order
  krsort($memories);
  return $memories;
}

/**
 * Process incoming SMS messages
 */
function _timenesia_sms_process_request($msg, $call_id) {
   $help_msg = t("Available commands: a) S NUMBER XXXXX (submit story XXXXX to location NUMBER); b) ? (this help message)");
  $msg = trim($msg);
  if (!$msg) {
    $ret = t("Empty request.") . ' ' . $help_msg;
  }
  else {
    $request = explode(" ", $msg);
    $cmd = strtoupper($request[0]);
    if ($cmd == '?') {
      $ret = $help_msg;
    }
    else if (count($request)<2) {
      $ret = t("Invalid request.") . ' ' . $help_msg;
    }
    else {
      $eid = $request[1];
      // validate extension number
      $extension = voipextension_load($eid);
      if(!$extension || ($extension['extension_type'] != 'destination') || !$extension['enabled']) {
        $ret = t('Invalid location number: @number', array('@number' => $eid));
        return($ret);
      }
      switch ($cmd) {
        default:
          $ret = t("Invalid request: $msg");
          break;
        case "S":
          // Submit story
          unset($request[0]);
          unset($request[1]);
          $text = implode(" ", $request);
            if(empty($text)) {
            return t("Story can't be empty.");
          }
          _timenesia_sms_create_memory($text, $eid, $call_id);
          $options = array('@eid' => $eid);
          $ret = t("Your story has been added to location @eid. Thanks! --the My Dot Tour team", $options);
          break;
      }
    }
  }
  return $ret;
}

/**
 * Create SMS memory
 */
function _timenesia_sms_create_memory($text, $location_id, $call_id) {
  $node->type = 'memory';
  $node->promote = FALSE;

  $extension = voipextension_load($location_id);
  $nid = $extension['module_id'];
  $node->memory_destination = $nid;

  $location_title = $extension['title'];

  $cid = VoipCall::getCidFromCallId($call_id);
  $call = VoipCall::load($cid);
  $caller_number = $call->getCallerNumber();

  $date = format_date(time());

  $options = array('@text' => $text, '@location_id' => $location_id, '@call_id' => $call_id, '@location_title' => $location_title, '@caller_number' => $caller_number, '@date' => $date);
  $node->title = t('New SMS memory for @location_title (@location_id) from @caller_number', $options);
  $body = t('Location title: @location_title, Location number: @location_id, SMS text: @text, Caller number: @caller_number, Date: @date, Call id: @call_id', $options);
  $node->body = $body;

  $node->field_memory_text[0]['value'] = $body;

  // create the node
  node_save($node);

  // TODO: send notification
}

