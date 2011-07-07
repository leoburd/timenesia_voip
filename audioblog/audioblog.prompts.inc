<?php

/**
 * Constant definitions
 */

// URL of the audio files

define('AUDIOBLOG_ENGLISH', 'http://duke.edu/~ml117/AUDIOBLOG/InteractiveSystem.wavfiles/English/');
define('AUDIOBLOG_SPANISH', 'http://duke.edu/~ml117/AUDIOBLOG/InteractiveSystem.wavfiles/Spanish/');


/**
 * Prompts
 */

// Messages that appear in more than one script

$invalid_msg = 'Invalid option selected';
$invalid_msg_es = 'Opción inválida.';


// Messages associated with the idepsca_287g_welcome script

$welcome_msg_en = "Welcome to Idepsca’s interactive 2 8 7 g and Secure Communities Abuse Report System.";
$welcome_msg_es = "Bievenido al sistema interactivo de idepsca para reporter abusos de la 2 8 7 g y Comunidades Seguras.";
$welcome_msg = "$welcome_msg_en $welcome_msg_es";
$welcome_msg = AUDIOBLOG_ENGLISH . "20110425_105216.mp3";

$choose_spanish_msg = "Para escuchar el resto del mensaje en Espanol, precione el 2.";
$choose_spanish_msg = AUDIOBLOG_SPANISH . "20110425_110937.mp3";


// Messages associated with the idepsca_287g_script script

$system_description = 'This system will allow you to record your experience with anti-immigration laws in the State of California. These recordings will allow you to share your story to idepsca in an effort to let your voice be included in the digital public sphere and create awareness around abuse of power. Please Note: idepsca is a grassroots organization that advocated for the rights of low-income workers in the state of California. We are not a law firm. Any wishes to resort to legal action regarding your case should be consulted with an attorney and NOT with idepsca. To learn more about idepsca, visit us at www.idepsca.org or call us at (213) 252-2952';
$system_description = AUDIOBLOG_ENGLISH . '20110425_111647.mp3';
$script_options = "To learn more about 287 g, Press 1. To learn more about Secure Communities, Press 2. To learn more about Special Order 40, Press 3. To begin recording your experience, Press 4. To listen to legal resources you can access, press 5.";
$script_options = AUDIOBLOG_ENGLISH . '20110425_111900.mp3'
                  . ' ' . AUDIOBLOG_ENGLISH . '20110425_111920.mp3'
                  . ' ' . AUDIOBLOG_ENGLISH . '20110425_111943.mp3'
                  . ' ' . AUDIOBLOG_ENGLISH . '20110425_112002.mp3'
                  . ' ' . AUDIOBLOG_ENGLISH . '20110425_112201.mp3';
$thank_you = "Thank you for using Idepsca's 2 8 7 g and Secure Communities Abuse Report System. To learn more about idepsca, please visit www.idepsca.org, or call us at (213) 252-2952.";
$thank_you = AUDIOBLOG_ENGLISH . '20110425_113210.mp3';

$system_description_es = 'Este sistema le permitirá grabar su experiencia con las leyes de anti-emigración en el estado de California. Esta grabación le permitirá compartir su historia con Idepsca como esfuerzo para incluir su voz en la esfera pública digital y documentar y exponer el abuso de poder. Por favor tome en cuenta: Idepsca es una organización de base que aboga por los derechos de trabajadores de bajos recursos en el estado de California. No somos un despacho de abogados. Si tiene algún deseo de tomar acción legal respecto a su caso, consúltelo con un abogado y NO con Idepsca. Para aprender mas sobre Idepsca, por favor visítenos en www.idepsca.org o llamenos al (213) 252-2952.';
$system_description_es = AUDIOBLOG_SPANISH . '20110425_113859.mp3';
$script_options_es = 'Para aprender más sobre la 287g, Presione 1. Para aprender más sobre Comunidades Seguras, Presione 2. Para aprender más sobre la Ordenanza Especial 40, Presione 3. Para empezar a grabar su experiencia, Presione 4. Para aprender sobre los recursos legales a los cuales pueda acudir, Presione 5.';
$script_options_es = AUDIOBLOG_SPANISH . '20110425_114044.mp3'
                    . ' ' . AUDIOBLOG_SPANISH . '20110425_114109.mp3'
                    . ' ' . AUDIOBLOG_SPANISH . '20110425_114126.mp3'
                    . ' ' . AUDIOBLOG_SPANISH . '20110425_114211.mp3'
                    . ' ' . AUDIOBLOG_SPANISH . '20110425_114228.mp3';

$thank_you_es = 'Gracias por usar el Sistema Interactivo del Comité Anti-Ejecución de Idepsca Para Reportar Abusos de la Polimigra. Para aprender más sobre Idepsca, por favor visite www.idepsca.org o llámenos al 213.252.2952';
$thank_you_es = AUDIOBLOG_SPANISH . '20110425_115957.mp3';


// Messages associated with the idepsca_287g_record_options script

$recording_note = 'Please note. If at any time you wish to stop this documentation process, simply hang up. All records completed prior to hanging up will be fully deleted from this system.';
$recording_note = AUDIOBLOG_ENGLISH . '20110425_112442.mp3';
$recording_menu = 'To record a story about an abuse by 2 8 7 g agents, Press 1. To record a story about an abuse by Secure Communities agents, Press 2. To record a story about an abuse by other law enforcement officers in California, Press 3.';
$recording_menu = 'To record a story about an abuse by 2 8 7 g agents, Press 1.'
                  . ' ' . AUDIOBLOG_ENGLISH . '20110425_112610.mp3'
                  . ' ' . AUDIOBLOG_ENGLISH . '20110425_112633.mp3';


$recording_note_es = 'Por favor note que si en cualquier momento de este proceso decide que ya no quiere compartir su experiencia, puede simplemente colgar. Todos los archivos grabados antes de colgar serán completamente borrados de este sistema.';
$recording_note_es = AUDIOBLOG_SPANISH . '20110425_114535.mp3';
$recording_menu_es = 'Para grabar una experiencia de un abuso de parte de un agente de la 287g, Presione 1. Para grabar una experiencia de un abuso de parte de un agente de Comunidades Seguras, Presione 2. Para grabar una experiencia sobre un abuso de parte de cualquier otro agente en el estado de California, Presione 3.';
$recording_menu_es = AUDIOBLOG_SPANISH . '20110425_114655.mp3'
                    . ' ' . AUDIOBLOG_SPANISH . '20110425_114715.mp3'
                    . ' ' . AUDIOBLOG_SPANISH . '20110425_114732.mp3';


// Messages associated with the idepsca_287g_record_story script

$recording_prompt = 'At the beep, please record your story and, if possible, include the date, time and location of your incident. When you are finished, press the pound key.';
$recording_prompt = AUDIOBLOG_ENGLISH . '20110425_112729.mp3';
$recording_confirmation = 'You will hear a playback of your recording. If you are satisfied with your recording, press pound. If you would like to re-record, press the star key.';
$recording_confirmation = AUDIOBLOG_ENGLISH . '20110425_113038.mp3';
$final_confirmation = "You are about to fully include your story in AUDIOBLOG's archives. Please note that by agreeing to turn in your recording, you are giving up your right of property of the recording. AUDIOBLOG will now have the right to share your story. If you agree with this arrangement, press 1. If you disagree and would like all recordings to be erased, press 2.";
$final_confirmation = AUDIOBLOG_ENGLISH . '20110425_113119.mp3';
$story_accepted = 'Your story has been successfully recorded';
$story_rejected = 'Your story has been deleted from the system';

$recording_prompt_es = "Al oír el pitido, por favor empiece a grabar su experiencia y, si es posible, empiece con incluir la fecha, tiempoy lugar del incidente. Cuando haya terminado, presione el signo numeral.";
$recording_prompt_es = AUDIOBLOG_SPANISH . '20110425_115213.mp3';
$recording_confirmation_es = "Ahora escuchara una reproducción de la grabación. Si está satisfecho/a con la grabación, presione el signo numeral (#). Si desea volver a grabar desde el principio, presione la estrella (*).";
$recording_confirmation_es = AUDIOBLOG_SPANISH . '20110425_115601.mp3';
$final_confirmation_es = "Usted está a punto de completamente incluir su experiencia en los archivos de AUDIOBLOG. Por favor note que al entregar su grabación, usted está renunciando a los derechos de propiedad de la grabación. AUDIOBLOG entonces tendrá el derecho de compartir su historia. Si usted está de acuerdo con esto, presione 1. Si no está de acuerdo y quiere borrar todas las grabaciones, presione el 2."; 
$final_confirmation_es = AUDIOBLOG_SPANISH . '20110425_115723.mp3';
$story_accepted_es = 'Experiencia grabada en el sistema.';
$story_rejected_es = 'Grabación removida del sistema.';

