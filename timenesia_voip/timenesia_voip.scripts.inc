<?php


/**
 * Implementation of hook_voipscript_get_script_names()
 */
function timenesia_voip_voipscript_get_script_names() {
  $script_names[] = 'timenesia_welcome';
  $script_names[] = 'timenesia_play_destination';
  $script_names[] = 'timenesia_record_options';
  $script_names[] = 'timenesia_record_memory';
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
//        $title = voipnode_get_title($node);
        $description = voipnode_get_description($node);
        $greeting = voipnode_get_greeting($node);
//        $gosub = voipextension_get_script($extension);
//        $script->addGosub($gosub['script_name'], $gosub['script_arguments']);

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
        $script->addGosub('timenesia_record_options', $options);
        $script->addGoto('destination_options');

        $script->addLabel('listen_to_memories');
$script->addSay('This is where callers will be able to browse through existing memories.');
        $script->addGoto('destination_options');

        $script->addLabel('invalid_option');
        $script->addGoto('go_back');

        $script->addLabel('go_back');
      }

      break;

    case 'timenesia_record_options':
      $script = new VoipScript('timenesia_record_options');

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
      $script->addGosub('timenesia_record_memory', $options);
      $script->addGoto('go_back');

      $script->addLabel('present');
      $script->addSay($p_record_present);
      $options['memory_type'] = 'present';
      $script->addGosub('timenesia_record_memory', $options);
      $script->addGoto('go_back');

      $script->addLabel('future');
      $script->addSay($p_record_future);
      $options['memory_type'] = 'future';
      $script->addGosub('timenesia_record_memory', $options);
      $script->addGoto('go_back');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();

      break;

    case 'timenesia_record_memory':
      $script = new VoipScript('timenesia_record_memory');

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
  }

  return $script;
}

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
