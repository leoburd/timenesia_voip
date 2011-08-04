<?php

/**
 * Constant definitions
 */

define('TIMENESIA_AUDIO_URL', 'http://whatsupserver.media.mit.edu/public/timenesia/voice_prompts/');

/**
 * Global variables
 */

global $timenesia_prompts;

/**
 * Function definitions
 */

function v($index, $language='en') {
  global $timenesia_prompts;
if(!$timenesia_prompts[$index]) { watchdog('debug', "missing $index"); }
  $file_name = $timenesia_prompts[$index][$language]['file_name'];
  if($file_name) {
    $rc = TIMENESIA_AUDIO_URL . $file_name;
  }
  else {
    $rc = $timenesia_prompts[$index][$language]['text'];
  }
  return $rc;
}

function timenesia_get_missing_prompt_recordings_list($language='en') {
  global $timenesia_prompts;
  foreach($timenesia_prompts as $prompt) {
    if(!$prompt[$language]['file_name']) {
      $list[] = $prompt[$language]['text'];
    } 
  }
  return $list;
}


/**
 * Prompts
 */

// Messages that appear in more than one script

$timenesia_prompts['p_invalid_msg']['en']['text'] = 'Invalid option selected';
$timenesia_prompts['p_invalid_msg']['en']['file_name'] = 'p_invalid_msg.mp3';


// Prompts associated with the timenesia_welcome script

$timenesia_prompts['p_system_greetings']['en']['text'] = 'Hello and welcome to My Dot Tour: an interactive tour of Fields Corners past, present, and future. ';

$timenesia_prompts['p_system_menu']['en']['text'] = "Main menu. Please type your location number. For a list of existing locations, press 0. To learn more about the My Dot Tour initiative, press the star key. ";
$timenesia_prompts['p_system_menu']['en']['file_name'] = "p_system_menu.mp3";

$timenesia_prompts['p_try_again']['en']['text'] = 'No input received. Please try again. ';
//$timenesia_prompts['p_try_again']['en']['file_name'] = 'p_try_again2.mp3';
$timenesia_prompts['p_try_again']['en']['file_name'] = 'p_enter_again.mp3';

$timenesia_prompts['p_system_description']['en']['text'] = ' My Dot Tour is an interactive multimedia tour centered on the area of Fields Corner in Dorchester. Follow the tour to hear stories of notable places of the past, present, and future of Fields Corner. The tour stops were selected and researched by local youth with help from archives, residents, and local institutions. Hear to the voices of Fields Corner, then add yours to our growing tale of the neighborhood.  This project is organized by the Fields Corner Collaborative in collaboration with the MIT Center for Civic Media. Learn more at www dot my dot tour dot org. ';

$timenesia_prompts['p_directory_name']['en']['text'] = "My Dot Tour location's directory. ";
$timenesia_prompts['p_directory_name']['en']['file_name'] = 'p_my_dot_tour_directory.mp3';

$timenesia_prompts['p_directory_instructions']['en']['text'] = "At any time, press the desired extension number or press the pound key to go back to the previous menu. ";
$timenesia_prompts['p_directory_instructions']['en']['file_name'] = 'p_at_any_time.mp3';

$timenesia_prompts['p_invalid_extension']['en']['text'] = 'Invalid extension number. Please try again. ';
$timenesia_prompts['p_invalid_extension']['en']['file_name'] = 'p_invalid_try_again.mp3';

$timenesia_prompts['p_system_description_menu']['en']['text'] = 'Press 1 to replay this message. Or press the pound key to go back to the main menu and begin your tour!';
$timenesia_prompts['p_system_description_menu']['en']['file_name'] = 'p_system_description_menu.mp3';

$timenesia_prompts['p_thank_you']['en']['text'] = "Thank you for using the My Dot Tour system.";
$timenesia_prompts['p_thank_you']['en']['file_name'] = 'p_thank_you.mp3';


// Prompts associated with the timenesia_play_destination script

$timenesia_prompts['p_invalid_location']['en']['text'] = 'Invalid location number. ';
$timenesia_prompts['p_invalid_location']['en']['file_name'] = 'p_invalid_location.mp3';

$timenesia_prompts['p_transferring_to_location']['en']['text'] = 'Transferring to location number ';
$timenesia_prompts['p_transferring_to_location']['en']['file_name'] = 'p_transferring_to_location.mp3';

$timenesia_prompts['p_destination_menu']['en']['text'] = 'To learn more about this location, type 1. To record a story, Press 2. To listen to existing stories, Press 3. To go back to the main menu, press the pound key.';
$timenesia_prompts['p_destination_menu']['en']['file_name'] = 'p_destination_menu.mp3';


// Prompts associated with the timenesia_record_options menu

$timenesia_prompts['p_recording_menu']['en']['text'] = 'To record a story about the past, Press 1. To record a story about the present, Press 2. To record a story about the future, Press 3. To go back to the main menu, press the pound key.';
$timenesia_prompts['p_recording_menu']['en']['file_name'] = 'p_recording_menu.mp3';

$timenesia_prompts['p_record_past']['en']['text'] = 'About to record a story about the past. ';
$timenesia_prompts['p_record_past']['en']['file_name'] = 'p_record_past.mp3';

$timenesia_prompts['p_record_present']['en']['text'] = 'About to record a story about the present. ';
$timenesia_prompts['p_record_present']['en']['file_name'] = 'p_record_present.mp3';

$timenesia_prompts['p_record_future']['en']['text'] = 'About to record a story about the future. ';
$timenesia_prompts['p_record_future']['en']['file_name'] = 'p_record_future.mp3';


// Prompts associated with the timenesia_record_memory script

$timenesia_prompts['p_recording_prompt']['en']['text'] = 'Please record your name and your story after the beep. When done, press the pound key.';
//$timenesia_prompts['p_recording_prompt']['en']['file_name'] = 'p_recording_prompt.mp3';
$timenesia_prompts['p_recording_prompt']['en']['file_name'] = 'p_record_name_story.mp3';
$timenesia_prompts['p_recording_confirmation']['en']['text'] = 'To hear your recording, press 1. To accept your recording, press 2. To record once again, press the star key.';
$timenesia_prompts['p_recording_confirmation']['en']['file_name'] = 'p_recording_confirmation.mp3';
$timenesia_prompts['p_recording_playback']['en']['text'] = 'This is what you recorded: ';
$timenesia_prompts['p_recording_playback']['en']['file_name'] = 'p_recording_playback.mp3';
$timenesia_prompts['p_story_accepted']['en']['text'] = 'Your story has been successfully recorded';
$timenesia_prompts['p_story_accepted']['en']['file_name'] = 'p_story_accepted.mp3';

// Prompts associated with play_memory script

$timenesia_prompts['p_next_memory_menu']['en']['text'] = 'Press any key to move to the next story, or press pound to go back to ';
$timenesia_prompts['p_next_memory_menu']['en']['file_name'] = 'p_move_to_next_story.mp3';

$timenesia_prompts['no_more_memories_to_play']['en']['text'] = 'There are no more stories to be played for ';
$timenesia_prompts['no_more_memories_to_play']['en']['file_name'] = 'p_no_more_stories.mp3';

