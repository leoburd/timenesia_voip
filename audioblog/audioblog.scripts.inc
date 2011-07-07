<?php

foreach (array('audioblog.prompts.inc') as $file) {
  require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . $file);
}

/**
 * Implementation of hook_voipscript_get_script_names()
 */
function audioblog_voipscript_get_script_names() {
  $script_names[] = 'audioblog_welcome';
  $script_names[] = 'audioblog_script';
  $script_names[] = 'audioblog_record_options';
  $script_names[] = 'audioblog_record_story_script';
  $script_names[] = 'audioblog_record_story_sub';
  return $script_names;
}

/**
 * Implementation of hook_voipscript_load_script()
 */
function audioblog_voipscript_load_script($script_name, $options=NULL) {

  $script = NULL;
  switch($script_name) {
    default:
      break;

    case 'audioblog_welcome':
      $options['language'] = 'English';

      $script = new VoipScript('audioblog_welcome');
      $script->addSetVoice('woman');

      $script->addSay($welcome_msg);

      $script->addLabel('language_selection');
      $prompt = new VoipPrompt($choose_spanish_msg, NULL, 'es');
      $script->addGetInput($prompt, 2, '');
      $script->addGotoIf('spanish_version', "^%input_digits == '2'");

      $script->addLabel('english_version');
      $script->addGosub('audioblog_script', $options);
      $script->addGoto('hang up');

      $script->addLabel('spanish_version');
      $options['language'] = 'Spanish';
      $script->addGosub('audioblog_script', $options);
      $script->addGoto('hang up');

      $script->addLabel('hang up');
      $script->addHangup();

      break;

    case 'audioblog_script':
      $script = new VoipScript('audioblog_script');
      if($options['language'] == 'Spanish') {
        $script->addSetLanguage('es');
        $system_description = $system_description_es;
        $script_options = $script_options_es;
        $invalid_msg = $invalid_msg_es;
        $thank_you = $thank_you_es;
      }

      $prompt = new VoipPrompt($system_description, NULL, NULL, 'digits');
      $script->addSay($prompt);

      $script->addLabel('script_options');
      $input_options = array(
        '1' => 'about_287g',
        '2' => 'about_secure_communities',
        '3' => 'about_special_order_40',
        '4' => 'experience_recording',
        '5' => 'legal_resources',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($script_options, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('about_287g');
      $script->addSay('Describing 287 g');
      $script->addGoto('script_options');

      $script->addLabel('about_secure_communities');
      $script->addSay('Describing Secure Communities');
      $script->addGoto('script_options');

      $script->addLabel('about_special_order_40');
      $script->addSay('Describing Special Order 40');
      $script->addGoto('script_options');

      $script->addLabel('experience_recording');
      $script->addGosub('audioblog_record_options', $options);
      $script->addGoto('go_back');

      $script->addLabel('legal_resources');
      $script->addSay('Describing legal resources');
      $script->addGoto('script_options');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addSay($thank_you, 'digits');
      $script->addReturn();

      break;

    case 'audioblog_record_options':
      $script = new VoipScript('audioblog_record_options');

      if ($options['language'] == 'Spanish') {
        $recording_note = $recording_note_es;
        $recording_menu = $recording_menu_es;
        $invalid_msg = $invalid_msg_es;
      }

      $prompt = new VoipPrompt($recording_note, NULL, NULL, 'digits');
      $script->addSay($prompt);

      $script->addLabel('recording_menu');
      $prompt = $recording_menu;
      $input_options = array(
        '1' => '287g_abuse',
        '2' => 'secure_communities_abuse',
        '3' => 'california_abuse',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($prompt, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('287g_abuse');
$script->addSay('About to record 287 g abuse');
      $options['story_type'] = '287g_abuse';
      $script->addGosub('audioblog_record_story_sub', $options);
      $script->addGoto('go_back');

      $script->addLabel('secure_communities_abuse');
$script->addSay('About to record Secure Communities abuse');
      $options['story_type'] = 'secure_communities_abuse';
      $script->addGosub('audioblog_record_story_sub', $options);
      $script->addGoto('go_back');

      $script->addLabel('california_abuse');
$script->addSay('About to record California abuse');
      $options['story_type'] = 'california_abuse';
      $script->addGosub('audioblog_record_story_sub', $options);
      $script->addGoto('go_back');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();

      break;

    case 'audioblog_record_story_script':
      $script = new VoipScript('audioblog_record_story_script');
$script->addSay('About to record a 287 g abuse story');
      $options['story_type'] = '287g_abuse';
      $script->addGosub('audioblog_record_story_sub', $options);
$script->addSay('Thanks you.');
      $script->addHangup();
      break;

    case 'audioblog_record_story_sub':
      $script = new VoipScript('audioblog_record_story_sub');

      if ($options['language'] == 'Spanish') {
        $recording_prompt = $recording_prompt_es;
        $recording_confirmation = $recording_confirmation_es;
        $final_confirmation = $final_confirmation_es;
        $story_accepted = $story_accepted_es;
        $story_rejected = $story_rejected_es;
        $invalid_msg = $invalid_msg_es;
      }

      $story_type = $options['story_type'];

      $script->addLabel('start');
      $prompt = $recording_prompt;
      $timeout = 5;
      $end_key = '#';
      $max_length = AUDIOBLOG_MAX_STORY_DURATION;
      $script->addRecord($prompt, $timeout, $end_key, $max_length);

      $script->addLabel('accept menu');
      $prompt = "$recording_confirmation. You said %recording_url"; 
      $input_options = array(
        '#' => 'accept_recording',
        '*' => 'start',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $script->addRunIvrMenu($prompt, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('accept_recording');
      $prompt = $final_confirmation;
      $input_options = array(
        '1' => 'save_recording',
        '2' => 'reject_recording',
        'i' => 'reject_recording',
        't' => 'reject_recording'
      );
      $script->addRunIvrMenu($prompt, $input_options, $invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('save_recording');
      $script->addSet('callback_result',
        "^audioblog_recording_callback('$story_type', %call_id, %recording_url, %recording_duration)");
      $script->addSay($story_accepted);
      $script->addGoto('go_back');

      $script->addLabel('reject_recording');
      $script->addSay($story_rejected);
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
      break;
  }

  return $script;
}


