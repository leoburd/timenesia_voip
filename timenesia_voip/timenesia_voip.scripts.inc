<?php


/**
 * Implementation of hook_voipscript_get_script_names()
 */
function timenesia_voip_voipscript_get_script_names() {
  $script_names[] = 'timenesia_welcome';
  $script_names[] = 'timenesia_record_options';
  $script_names[] = 'timenesia_record_story';
  return $script_names;
}

/**
 * Implementation of hook_voipscript_load_script()
 */
function timenesia_voip_voipscript_load_script($script_name, $options=NULL) {

  require(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'timenesia_voip.prompts.inc');

  $script = NULL;
  switch($script_name) {
    default:
      break;

    case 'timenesia_welcome':
      $options['language'] = 'English';

      $script = new VoipScript('timenesia_welcome');
      $script->addSetVoice('woman');

      $script->addSay($p_welcome_msg);

      $script->addLabel('main_menu');
      $script->addSet('try_again', FALSE);
      $input_options = array(
        '1' => 'record_audioblog',
        '2' => 'listen_to_audioblogs',
        '3' => 'learn_more',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($p_main_menu, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('record_audioblog');
      $script->addGosub('timenesia_record_options', $options);
      $script->addGotoIf('main_menu', '^%try_again == TRUE');
      $script->addGoto('go_back');

      $script->addLabel('listen_to_audioblogs');
$script->addSay('This is where callers will be able to browse through existing audio blog entries.');
      $script->addGoto('main_menu');

      $script->addLabel('learn_more');
      $script->addSay($p_system_description);
      $script->addGoto('main_menu');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addSay($p_thank_you);
      $script->addHangup();

      break;

    case 'timenesia_record_options':
      $script = new VoipScript('timenesia_record_options');

      $script->addLabel('recording_menu');
      $script->addSet('try_again', FALSE);
      $prompt = $p_recording_menu;
      $input_options = array(
        '1' => 'past',
        '2' => 'present',
        '3' => 'future',
        '#' => 'back_to_main_menu',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($prompt, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('past');
$script->addSay('About to record story about the past.');
      $options['story_type'] = 'past';
      $script->addGosub('timenesia_record_story', $options);
      $script->addGotoIf('recording_menu','^%try_again == TRUE');
      $script->addGoto('go_back');

      $script->addLabel('present');
$script->addSay('About to record a story about the present.');
      $options['story_type'] = 'present';
      $script->addGosub('timenesia_record_story', $options);
      $script->addGotoIf('recording_menu','^%try_again == TRUE');
      $script->addGoto('go_back');

      $script->addLabel('future');
$script->addSay('About to record a story about the future');
      $options['story_type'] = 'future';
      $script->addGosub('timenesia_record_story', $options);
      $script->addGotoIf('recording_menu','^%try_again == TRUE');
      $script->addGoto('go_back');

      $script->addLabel('back_to_main_menu');
      $script->addSet('try_again', TRUE);
      $script->addGoto('go_back');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();

      break;

    case 'timenesia_record_story':
      $script = new VoipScript('timenesia_record_story');

      $story_type = $options['story_type'];

      $script->addLabel('start');
      $prompt = $p_recording_prompt;
      $timeout = 5;
      $end_key = '#';
      $max_length = 120;
      $script->addRecord($prompt, $timeout, $end_key, $max_length);

      $script->addLabel('accept_menu');
      $prompt = $p_recording_confirmation; 
      $input_options = array(
        '1' => 'play_recording',
        '2' => 'accept_recording',
        '3' => 'start',
        '#' => 'back_to_recording_menu',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($prompt, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('play_recording');
      $script->addSay($p_recording_playback);
      $script->addSay('%recording_url');
      $script->addGoto('accept_menu');

      $script->addLabel('accept_recording');
      $script->addSet('callback_result',
//        "^timenesia_recording_callback(%call_id, %recording_url, %recording_duration, '$story_type')");
        "^audioblog_recording_callback(%call_id, %recording_url, %recording_duration, '$story_type', 'memory', 'field_memory_audio')");
      $script->addSay($p_story_accepted);
      $script->addGoto('go_back');

      $script->addLabel('back_to_recording_menu');
      $script->addSet('try_again', TRUE);
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
      break;
  }

  return $script;
}


