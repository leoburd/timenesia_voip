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

      $script->addSay($p_system_greetings);

      $script->addLabel('system_options');
      $input_options = array(
        '1' => 'go_to_destination',
        '2' => 'browse_destinations',
        '3' => 'learn_more',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($p_system_menu, $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('go_to_destination');
      $script->addGosub('timenesia_play_destination', $options);
      $script->addGoto('go_back');

      $script->addLabel('browse_destinations');
$script->addSay('This is where callers will browse through existing destinations.');
      $script->addGoto('system_options');

      $script->addLabel('learn_more');
      $script->addSay($p_system_description);
      $script->addGoto('system_options');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addSay($p_thank_you);
      $script->addHangup();

      break;

    case 'timenesia_play_destination':
      $script = new VoipScript('timenesia_play_destination');

      $script->addSay($p_destination_greetings);

      $script->addLabel('destination_options');
      $input_options = array(
        '1' => 'record_memory',
        '2' => 'listen_to_memories',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($p_destination_menu, $input_options);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('record_memory');
$options['memory_destination'] = 25;
      $script->addGosub('timenesia_record_options', $options);
      $script->addGoto('go_back');

      $script->addLabel('listen_to_memories');
$script->addSay('This is where callers will be able to browse through existing memories.');
      $script->addGoto('destination_options');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addSay($p_thank_you);
      $script->addHangup();

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
$script->addSay('About to record memory about the past.');
      $options['memory_type'] = 'past';
      $script->addGosub('timenesia_record_memory', $options);
      $script->addGoto('go_back');

      $script->addLabel('present');
$script->addSay('About to record a memory about the present.');
      $options['memory_type'] = 'present';
      $script->addGosub('timenesia_record_memory', $options);
      $script->addGoto('go_back');

      $script->addLabel('future');
$script->addSay('About to record a memory about the future');
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
      $audioblog_options['node_type'] = 'memory';
      $audioblog_options['audio_field_name'] = 'field_memory_audio';
      $audioblog_options['audioblog_type'] = $options['memory_type'];
      $audioblog_options['memory_destination'] = $options['memory_destination'];
      $serialized_options = serialize($audioblog_options);
      $script->addSet('callback_result',
        "^audioblog_recording_callback(%call_id, %recording_url, %recording_duration, '$serialized_options')");
      $script->addSay($p_story_accepted);
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
      break;
  }

  return $script;
}


