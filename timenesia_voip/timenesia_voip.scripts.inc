<?php


/**
 * Implementation of hook_voipscript_get_script_names()
 */
function timenesia_voip_voipscript_get_script_names() {
  $script_names[] = 'timenesia_welcome';
  $script_names[] = 'timenesia_play_destination_script';
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
watchdog('timenesia_voip', "(load_script) name: $script_name, options: ".print_r($options, true));

  require(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'timenesia_voip.prompts.inc');

  $script = NULL;
  switch($script_name) {
    default:
      break;

    case 'timenesia_welcome':
      $options['language'] = 'English';

      $script = new VoipScript('timenesia_welcome');
      $script->addSetVoice('woman');
      $script->addSet('max_session_duration', 0);

      $script->addSay($p_system_greetings);

      // read destination id
      $script->addSet('counter', 0); // number of attemtps

      $script->addLabel('system_options');
      $script->addGetInput($p_system_menu, NULL, '#', 3);

      $script->addSet('destination_id', '%input_digits');
      $script->addGotoIf('no_input_received', "^%input_digits == ''");
      $script->addGotoIf('browse_destinations', "^%input_digits == '0'");
      $script->addGotoIf('learn_more', "^%input_digits == '*'");
      $script->addGoto('play_destination');

      $script->addLabel('no_input_received');
      $script->addSet('counter', '^%counter + 1');
      $script->addGotoIf('go_back', '^%counter > 3');
      $script->addSay($p_try_again);
      $script->addGoto('system_options');

      $script->addLabel('play_destination');
      $options['destination_id'] = '%destination_id';
      $script->addGosub('timenesia_play_destination_script', $options);
      $script->addGoto('system_options');

      $script->addLabel('browse_destinations');
$script->addSay('This is where callers will browse through existing destinations.');
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
      $script->addRunIvrMenu(" $p_system_description $p_system_description_menu", $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addSay($p_thank_you);
      $script->addHangup();

      break;

    case 'timenesia_play_destination_script':
      $script = new VoipScript('timenesia_play_destination_script');
      $eid = $options['destination_id'];
      $extension = voipextension_load($eid);
      if (!$extension) {
        $script->addSay($p_invalid_location);
        $script->addSet('invalid_extension_number', TRUE);
      }
      else {
        $script->addSay($p_transferring_to_location . $eid . ' .');
        $script->addSet('invalid_extension_number', FALSE);
        $destination_nid = $extension['module_id'];
        $node = node_load($destination_nid);
        $description = voipnode_get_description($node);
        $greeting = voipnode_get_greeting($node);

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
        $script->addRunIvrMenu($p_destination_menu, $input_options);
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
      }

      break;

    case 'timenesia_record_options_script':
      $script = new VoipScript('timenesia_record_options_script');

      $script->addLabel('recording_menu');
      $prompt = $p_recording_menu;
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
      $script->addSay($p_record_past);
      $options['memory_type'] = 'past';
      $script->addGosub('timenesia_record_memory_script', $options);
      $script->addGoto('go_back');

      $script->addLabel('present');
      $script->addSay($p_record_present);
      $options['memory_type'] = 'present';
      $script->addGosub('timenesia_record_memory_script', $options);
      $script->addGoto('go_back');

      $script->addLabel('future');
      $script->addSay($p_record_future);
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
      $prompt = $p_recording_prompt;
      $timeout = 5;
      $end_key = '#';
      $max_length = AUDIOBLOG_MAX_STORY_DURATION;
      $script->addRecord($prompt, $timeout, $end_key, $max_length);

      $script->addLabel('accept_menu');
      $prompt = $p_recording_confirmation; 
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
      $script->addSay($p_recording_playback);
      $script->addSay('%recording_url');
      $script->addGoto('accept_menu');

      $script->addLabel('accept_recording');
      $serialized_options = serialize($options);
      $destination_nid = $options['destination_nid'];
      $script->addSet('callback_result',
        "^_timenesia_voip_recording_callback($destination_nid, %call_id, %recording_url, %recording_duration, '$serialized_options')");
// TODO: check callback result
      $script->addSay($p_story_accepted);
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
      break;

    case 'timenesia_play_memories_script':
      $script = _timenesia_voip_get_play_memories_script($options['destination_nid']);
      break;

    case 'timenesia_sms_handler_script':
      $script = new VoipScript('timenesia_sms_handler_script');
      $script->addSet('request_result', '^_timenesia_sms_parse_request(%inbound_text_contents, %caller_number)');
      $script->addSendText("%request_result");
      $script->addHangup();
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
      $script->addSay($memory['audio_url']);
$p_next_memory_menu = t('Press any key to move to the next story, or press pound to go back to @destination_name. ', array('@destination_name' => $destination_name));
      $script->addGetInput($p_next_memory_menu, 1, '', 3);
      $script->addGotoIf('go_back', '^%input_digits == "#"');
    }
$p_no_more_memories_to_play = t('There are no more stories to be played for @destination_name. ', array('@destination_name' => $destination_name));
    $script->addSay($p_no_more_memories_to_play);
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
function _timenesia_sms_parse_request($msg, $from) {
  $msg = trim($msg);
  if (!$msg) {
    $ret = t("Empty request received");
  }
  else {
    $ret = 'Thank you for contacting the My Dot Tour system. Stay tuned!';
  }
  return $ret;
}
